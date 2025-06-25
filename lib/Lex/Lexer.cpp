//===----------------------------------------------------------------------===//
// Lexer.cpp - Implementation of the Stone Lexer
//===----------------------------------------------------------------------===//

#include "stone/Lex/Lexer.h"
#include "stone/Core/CharInfo.h"
#include "stone/Core/DiagID.h"
#include "stone/Core/Unicode.h"
#include "stone/Lex/Confusable.h"
#include "stone/Lex/LexerObserver.h"

#include "clang/Basic/CharInfo.h"
#include "llvm/ADT/StringSwitch.h"
#include "llvm/Support/Unicode.h"

using namespace stone;
using namespace stone::diag;

//===----------------------------------------------------------------------===//
// Lexer - Constructors
//===----------------------------------------------------------------------===//

// void Lexer::ComputeOffsetsForUnit(SrcUnit &unit) {
//   assert(unit.IsValid() && "Invalid SrcUnit!");

//   unsigned bufferID = unit.GetBufferID();
//   unsigned length = sm.getRangeForBuffer(bufferID).getByteLength();

//   unit.SetBufferEndOffset(length);
//   // Can be changed later for lex-limiting
//   unit.SetEndOffset(length); // Default: lex full buffer
// }

// void Lexer::ComputeOffsetsForUnit(SrcUnit &unit, const SrcMgr &sm,
//                                   const LexerBookmark &begin,
//                                   const LexerBookmark &end) {
//   assert(unit.IsValid() && "Invalid SrcUnit!");

//   // Sanity checks to ensure bookmarks belong to this unit’s buffer
//   assert(sm.findBufferContainingLoc(begin.GetLoc()) == unit.GetBufferID() &&
//          "Begin location is not from the expected buffer!");
//   assert(sm.findBufferContainingLoc(end.GetLoc()) == unit.GetBufferID() &&
//          "End location is not from the expected buffer!");

//   // Compute physical byte offsets from source locations
//   unsigned beginOffset =
//       sm.getLocOffsetInBuffer(begin.GetLoc(), unit.GetBufferID());
//   unsigned endOffset =
//       sm.getLocOffsetInBuffer(end.GetLoc(), unit.GetBufferID());

//   // Record offsets for lexing limits
//   unit.SetBeginOffset(beginOffset);
//   unit.SetLimitOffset(endOffset);
// }

void Lexer::Init(SrcUnit &unit, SrcMgr &sm) {

  // Extract and store length-based offsets
  // ComputeOffsetsForUnit(unit);

  cursor.Clear();
  auto contents = sm.extractText(sm.getRangeForBuffer(unit.GetBufferID()));
  cursor.Init(contents.data(), contents.size());
  Lex();
  cursor.Check();

  // BufferStart = contents.data();
  // BufferEnd = BufferStart + contents.size();

  // assert(BufferEnd > BufferStart && "Invalid or empty buffer.");
  // assert(BufferStart + unit.GetOffset() <= BufferEnd &&
  //        "Start offset is out of bounds.");
  // assert(BufferStart + unit.GetEndOffset() <= BufferEnd &&
  //        "End offset is out of bounds.");

  // ContentStart = llvm::StringRef(BufferStart, 3).equals("\xEF\xBB\xBF")
  //                    ? BufferStart + 3
  //                    : BufferStart;

  // CurPtr = BufferStart + unit.GetOffset();
  // ArtificialEOF = BufferStart + unit.GetEndOffset();

  // if (unit.GetBufferID() == sm.getCodeCompletionBufferID()) {
  //   const char *Ptr = BufferStart + sm.getCodeCompletionOffset();
  //   if (Ptr >= BufferStart && Ptr <= BufferEnd) {
  //     CodeCompletionPtr = Ptr;
  //   }
  // }

  // assert(nextToken.Is(tok::LAST));
  // Lex();
  // assert((nextToken.IsAtStartOfLine() || CurPtr != BufferStart) &&
  //        "Token should begin on a fresh line or a non-zero offset.");
}

Lexer::Lexer(SrcUnit &unit, SrcMgr &sm) : unit(unit), sm(sm) {
  assert(unit && "SrcUnit has an invalid buffer-id!");
  // Proceed with lex setup
  Init(unit, sm);
}

// Lexer::Lexer(Lexer &parent, LexerBookmark begin, LexerBookmark end)
//     : unit(parent.GetSrcUnit()), sm(parent.GetSrcMgr()) {

//   SetBufferStart(parent.GetBufferStart());
//   SetBufferEnd(parent.GetBufferEnd());
//   SetContentStart(parent.GetContentStart());
//   SetCurPtr(parent.GetCurPtr());
//   SetArtificialEOF(parent.GetArtificialEOF());

//   ComputeOffsetsForUnit(parent.GetSrcUnit(), parent.GetSrcMgr(), begin, end);

//   // Record offsets for future reference
//   unsigned beginOffset =
//       sm.getLocOffsetInBuffer(begin.GetLoc(), unit.GetBufferID());
//   unsigned endOffset =
//       sm.getLocOffsetInBuffer(end.GetLoc(), unit.GetBufferID());

//   unit.SetBeginOffset(beginOffset);
//   unit.SetEndOffset(endOffset);

//   nextToken.SetKind(tok::LAST);
// }

/// Pull API entrypoint
void Lexer::Lex(Token &result) {
  result = nextToken;
  if (!result.IsEOF()) {
    Lex(); // Advance to next token
  }
}

void Lexer::Lex() {
  cursor.Check();
  if (cursor.CurPtr == cursor.InitialBuffer) {
    cursor.SkipUTF8BOM();
    nextToken.SetAtStartOfLine(true);
  } else {
    nextToken.SetAtStartOfLine(false);
  }

  Trivia trivia = LexTrivia(cursor);
}
// clang-format off
// void Lexer::Lex() {
//   assert(HasValidBufferBounds() && "Lexer cursor is out of buffer bounds!");

//   const char *leadingTriviaStart = CurPtr;

//   // Skip BOM at start
//   if (CurPtr == BufferStart) {
//     if (BufferStart < ContentStart) {
//       size_t bomLen = ContentStart - BufferStart;
//       assert(bomLen == 3 && "UTF-8 BOM must be 3 bytes");
//       CurPtr += bomLen;
//     }
//   nextToken.SetAtStartOfLine(true);
//   } else {
//   nextToken.SetAtStartOfLine(false);
//   }


//   Trivia trivia = LexTrivia(/*isForTrailingTrivia=*/false, leadingTriviaStart);
//   if (trivia.kind != TriviaKind::None && unit.triviaKind != TriviaKind::None) {
//     unit.AddTrivia(trivia);
//   }

//   const char *tokStart = CurPtr;

//   if (LexerCutoffPoint && CurPtr >= LexerCutoffPoint) {
//     return FormToken(tok::eof, tokStart);
//   }

//   switch (*CurPtr++) {
//   default: {
//     const char *tmp = CurPtr - 1;

//     if (AdvanceIfValidIdentifierStart(tmp, BufferEnd))
//       return LexIdentifier();

//     if (AdvanceIfValidOperatorStart(tmp, BufferEnd))
//       return LexOperator();

//     bool ok = LexUnknown(/*track=*/true);
//     assert(ok && "UTF-8 errors must be handled via trivia");
//     return FormToken(tok::alien, tokStart);
//   }

//   case '\n': case '\r': case ' ': case '\t':
//   case '\f': case '\v':
//     llvm_unreachable("Trivia should have consumed whitespace/newlines");

//   case (char)-1:
//   case (char)-2:
//     Track(diag::DiagID::UTF16BOMMarker);
//     CurPtr = BufferEnd;
//     return FormToken(tok::alien, tokStart);

//   case 0:
//     switch (GetNullCharacterKind(CurPtr - 1)) {
//     case NullCharacterKind::CodeCompletion:
//       while (AdvanceIfValidIdentifierContinue(CurPtr, BufferEnd)) {
//       }
//       return FormToken(tok::code_complete, tokStart);
//     case NullCharacterKind::BufferEnd:
//       --CurPtr;
//       return FormToken(tok::eof, tokStart);
//     case NullCharacterKind::Embedded:
//       llvm_unreachable("Embedded nulls must be trivia-handled");
//     }

//   // Delimiters
//   case '@':  return FormToken(tok::at_sign, tokStart);
//   case '{':  return FormToken(tok::l_brace, tokStart);
//   case '[':  return FormToken(tok::l_square, tokStart);
//   case '(':  return FormToken(tok::l_paren, tokStart);
//   case '}':  return FormToken(tok::r_brace, tokStart);
//   case ']':  return FormToken(tok::r_square, tokStart);
//   case ')':  return FormToken(tok::r_paren, tokStart);
//   case ',':  return FormToken(tok::comma, tokStart);
//   case ';':  return FormToken(tok::semi, tokStart);
//   case ':':  return FormToken(tok::colon, tokStart);
//   case '\\': return FormToken(tok::backslash, tokStart);

//   // Hash / String
//   case '#':
//     if (unsigned delimLen = AdvanceIfCustomDelimiter(CurPtr, stringDelim)) {
//       return LexStringLiteral(delimLen);
//     }
//     return LexHash();

//   // Comments and operators
//   case '/':
//     if (CurPtr[0] == '/') {
//       SkipLineComment(/*eatNewline=*/true);
//       assert(IsKeepingComments());
//       return FormToken(tok::comment, tokStart);
//     }
//     if (CurPtr[0] == '*') {
//       SkipBlockComment();
//       assert(IsKeepingComments());
//       return FormToken(tok::comment, tokStart);
//     }
//     return LexOperator();

//   case '%': case '>': case '=': case '-': case '+':
//   case '*': case '&': case '|': case '^': case '~': case '.':
//     return LexOperator();

//   // Postfix operators
//   case '!':{
//     if (IsLeftBound(tokStart, ContentStart)){
//       return FormToken(tok::exclaim_postfix, tokStart);
//     }
//     return LexOperator();
//   }

//   case '?':{
//     if (IsLeftBound(tokStart, ContentStart)){
//       return FormToken(tok::question_postfix, tokStart);
//     }
//     return LexOperator();
//   }

//   case '<': {
//     if (CurPtr[0] == '#'){
//       return TryLexEditorPlaceholder();
//     }
//     return LexOperator();
//    }

//   // Identifiers
//   case 'A': case 'B': case 'C': case 'D': case 'E': case 'F':
//   case 'G': case 'H': case 'I': case 'J': case 'K': case 'L':
//   case 'M': case 'N': case 'O': case 'P': case 'Q': case 'R':
//   case 'S': case 'T': case 'U': case 'V': case 'W': case 'X':
//   case 'Y': case 'Z':
//   case 'a': case 'b': case 'c': case 'd': case 'e': case 'f':
//   case 'g': case 'h': case 'i': case 'j': case 'k': case 'l':
//   case 'm': case 'n': case 'o': case 'p': case 'q': case 'r':
//   case 's': case 't': case 'u': case 'v': case 'w': case 'x':
//   case 'y': case 'z':
//   case '_':{
//     return LexIdentifier();
//   }

//   case '$':{
//     return LexDollarIdent();
//   }

//   // Numbers
//   case '0': case '1': case '2': case '3': case '4':
//   case '5': case '6': case '7': case '8': case '9':{
//     return LexNumber();
//   }

//   // Strings
//   case '\'': case '"':
//     return LexStringLiteral();

//   case '`':
//     return LexEscapedIdentifier();
//   }
// }
// // clang-format on
// void Lexer::Lex(TokenCallback callback) {
//   Token tok;
//   do {
//     Lex(tok);
//     callback(tok);
//   } while (tok.GetKind() != tok::eof);
// }

// void Lexer::SeekTo(size_t offset) { CurPtr = BufferStart + offset; }

// size_t Lexer::GetOffset() const {
//   return static_cast<size_t>(CurPtr - BufferStart);
// }

// const char *Lexer::GetPtrForLoc(SrcLoc loc) const {
//   return loc.GetPtr(); // or validate with SrcMgr if needed
// }

// //===----------------------------------------------------------------------===//
// // Token Formation and Trivia — Stubbed for now
// //===----------------------------------------------------------------------===//

// void Lexer::FormToken(tok kind, const char *tokStart) {
//   // TODO
// }

// void Lexer::FormEscapedIdentifierToken(const char *tokStart) {
//   // TODO
// }

// void Lexer::FormStringLiteralToken(const char *tokStart, bool isMultiline,
//                                    unsigned customDelimiterLen) {
//   // TODO
// }




Trivia Lexer::LexTrivia(Cursor& cursor, bool isForTrailingTrivia){

   while (cursor.HasNext()) {
    switch(cursor.Peek()){
    case ' ':
      return Trivia{TriviaKind::Space, llvm::StringRef(cursor.GetCurPtr(), 1)};
    case '\t':
      return Trivia{TriviaKind::Tab, llvm::StringRef(cursor.GetCurPtr(), 1)};
    case '\v':
    case '\f':
      return Trivia{TriviaKind::Space, llvm::StringRef(cursor.GetCurPtr(), 1)};
    case '\n':
      if (!isForTrailingTrivia)
        nextToken.SetAtStartOfLine(true);
      return Trivia{TriviaKind::Newline, llvm::StringRef(cursor.GetCurPtr(), 1)};
    }
   }
return Trivia{TriviaKind::None, llvm::StringRef()};

}
//Trivia Lexer::LexTrivia(const char *allTriviaStart, bool isForTrailingTrivia) {
  //CommentStart = nullptr;

  // while (CurPtr < BufferEnd) {
  //   const char *triviaStart = CurPtr;
  //   char c = *CurPtr++;

  //   switch (c) {
  //   case ' ':
  //     return Trivia{TriviaKind::Space, llvm::StringRef(triviaStart, 1)};
  //   case '\t':
  //     return Trivia{TriviaKind::Tab, llvm::StringRef(triviaStart, 1)};
  //   case '\v':
  //   case '\f':
  //     return Trivia{TriviaKind::Space, llvm::StringRef(triviaStart, 1)};
  //   case '\n':
  //     if (!isForTrailingTrivia)
  //       nextToken.SetAtStartOfLine(true);
  //     return Trivia{TriviaKind::Newline, llvm::StringRef(triviaStart, 1)};
  //   case '\r':
  //     if (!isForTrailingTrivia)
  //       nextToken.SetAtStartOfLine(true);
  //     if (CurPtr < BufferEnd && *CurPtr == '\n')
  //       ++CurPtr;
  //     return Trivia{TriviaKind::Newline,
  //                   llvm::StringRef(triviaStart, CurPtr - triviaStart)};

  //   case '/':
  //     if (isForTrailingTrivia || IsKeepingComments())
  //       break;

  //     if (CurPtr[0] == '/') {
  //       if (!CommentStart)
  //         CommentStart = CurPtr - 1;
  //       bool isDoc = CurPtr[1] == '/';
  //       const char *commentStart = CurPtr - 1;
  //       SkipLineComment(/*eatNewline=*/false);
  //       return Trivia{isDoc ? TriviaKind::DocComment : TriviaKind::LineComment,
  //                     llvm::StringRef(commentStart, CurPtr - commentStart)};
  //     } else if (CurPtr[0] == '*') {
  //       if (!CommentStart)
  //         CommentStart = CurPtr - 1;
  //       bool isDoc = CurPtr[1] == '*';
  //       const char *commentStart = CurPtr - 1;
  //       SkipBlockComment();
  //       return Trivia{isDoc ? TriviaKind::DocComment : TriviaKind::BlockComment,
  //                     llvm::StringRef(commentStart, CurPtr - commentStart)};
  //     }
  //     break;

  //   case '#':
  //     if (triviaStart == ContentStart && CurPtr[0] == '!') {
  //       --CurPtr; // rewind to '#'
  //       if (!IsHashbangAllowed())
  //         Diagnose(triviaStart, DiagID::HashbangNotAllowed);
  //       SkipHashbang(/*eatNewline=*/false);
  //       return Trivia{TriviaKind::LineComment,
  //                     llvm::StringRef(triviaStart, CurPtr - triviaStart)};
  //     }
  //     break;

  //   case '<':
  //   case '>':
  //     if (TryLexConflictMarker(/*EatNewline=*/false)) {
  //       return Trivia{TriviaKind::LineComment,
  //                     llvm::StringRef(triviaStart, CurPtr - triviaStart)};
  //     }
  //     break;

  //   case 0:
  //     switch (GetNullCharacterKind(CurPtr - 1)) {
  //     case NullCharacterKind::Embedded:
  //       DiagnoseEmbeddedNul(CurPtr - 1);
  //       continue; // treat as ignorable trivia
  //     case NullCharacterKind::CodeCompletion:
  //     case NullCharacterKind::BufferEnd:
  //       break;
  //     }
  //     break;

  //   default: {
  //     const char *tmp = CurPtr - 1;
  //     if (AdvanceIfValidStartOfIdentifier(tmp, BufferEnd))
  //       break;

  //     if (AdvanceIfValidStartOfOperator(tmp, BufferEnd))
  //       break;

  //     bool shouldTokenize = LexUnknown(/*track=*/false);
  //     if (shouldTokenize) {
  //       CurPtr = tmp;
  //       return Trivia{TriviaKind::None,
  //                     llvm::StringRef(allTriviaStart, CurPtr - allTriviaStart)};
  //     }
  //     continue;
  //   }
  //   }
  //   --CurPtr; // Rewind one byte if we exited switch without advancing
  //   break;
  //}

//   return Trivia{TriviaKind::None, llvm::StringRef()};
// }




// //===----------------------------------------------------------------------===//
// // StringSegment helpers
// //===----------------------------------------------------------------------===//

// StringSegment StringSegment::GetLiteral(SrcLoc loc, unsigned length,
//                                         bool isFirst, bool isLast,
//                                         unsigned indentToStrip,
//                                         unsigned customDelimiterLen) {
//   return {Literal, loc,   length, indentToStrip, customDelimiterLen,
//           isFirst, isLast};
// }

// StringSegment StringSegment::GetExpr(SrcLoc loc, unsigned length) {
//   return {Expr, loc, length, 0, 0, false, false};
// }

// SrcLoc StringSegment::GetEndLoc() const { return Loc.GetAdvancedLoc(Length); }

// //===----------------------------------------------------------------------===//
// // Cutoff Utilities and Optional Helpers
// //===----------------------------------------------------------------------===//

// void Lexer::CutOffLexing() { LexerCutOffPoint = CurPtr; }

// llvm::StringRef Lexer::GetEncodedStringSegmentImpl(
//     llvm::StringRef str, llvm::SmallVectorImpl<char> &buffer,
//     bool isFirstSegment, bool isLastSegment, unsigned indentToStrip,
//     unsigned customDelimiterLen) {
//   // TODO: implement
//   return {};
// }

// void Lexer::SkipToEndOfLine(bool EatNewline) {
//   while (CurPtr < BufferEnd) {
//     char c = *CurPtr;
//     if (c == '\n') {
//       if (EatNewline)
//         ++CurPtr;
//       return;
//     }
//     if (c == '\r') {
//       if (EatNewline) {
//         ++CurPtr;
//         if (CurPtr < BufferEnd && *CurPtr == '\n')
//           ++CurPtr;
//       }
//       return;
//     }
//     ++CurPtr;
//   }
// }

// void Lexer::SkipHashbang(bool EatNewline) {
//   assert(CurPtr[0] == '#' && CurPtr[1] == '!' && "Not a valid hashbang start");
//   SkipToEndOfLine(EatNewline);
// }

// void Lexer::SkipLineComment(bool EatNewline) {
//   while (CurPtr < BufferEnd) {
//     char c = *CurPtr;
//     if (c == '\r' || c == '\n') {
//       if (EatNewline) {
//         if (c == '\r' && CurPtr + 1 < BufferEnd && CurPtr[1] == '\n')
//           ++CurPtr;
//         ++CurPtr;
//       }
//       return;
//     }
//     ++CurPtr;
//   }
// }

// void Lexer::SkipBlockComment() {
//   while (CurPtr < BufferEnd) {
//     if (CurPtr[0] == '*' && CurPtr + 1 < BufferEnd && CurPtr[1] == '/') {
//       CurPtr += 2; // consume */
//       return;
//     }
//     ++CurPtr;
//   }
//   // Unterminated block comment — optionally diagnose here.
//   // Diagnose(CurPtr - 1, DiagID::UnterminatedBlockComment);
// }

// bool Lexer::AdvanceIf(const char *&ptr, const char *end,
//                       bool (*predicate)(uint32_t)) {
//   const char *start = ptr;
//   uint32_t codePoint = unicode::ValidateUTF8CharacterAndAdvance(ptr, end);
//   if (codePoint == ~0U)
//     return false;
//   if (predicate(codePoint))
//     return true;
//   ptr = start; // rewind
//   return false;
// }

// bool Lexer::AdvanceIfValidStartOfIdentifier(const char *&ptr, const char *end) {
//   return AdvanceIf(ptr, end, stone::charinfo::IsIdentifierStart);
// }

// bool Lexer::AdvanceIfValidContinuationOfIdentifier(const char *&ptr,
//                                                    const char *end) {
//   return AdvanceIf(ptr, end, stone::charinfo::IsIdentifierContinuation);
// }

// bool Lexer::AdvanceIfValidStartOfOperator(const char *&ptr, const char *end) {
//   return AdvanceIf(ptr, end, stone::charinfo::IsOperatorStart);
// }

// bool Lexer::AdvanceIfValidContinuationOfOperator(const char *&ptr,
//                                                  const char *end) {
//   return AdvanceIf(ptr, end, stone::charinfo::IsOperatorContinuation);
// }


void CursorRange::Finish(const Cursor &cursor) { end = cursor.GetCurPtr(); }
