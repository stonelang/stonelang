//===----------------------------------------------------------------------===//
// Lexer.h - Source File Lexer Interface
//===----------------------------------------------------------------------===//

#ifndef STONE_LEX_LEXER_H
#define STONE_LEX_LEXER_H

#include "stone/Core/DiagID.h"
#include "stone/Core/SrcMgr.h"
#include "stone/Core/SrcUnit.h"

#include "stone/Lex/BufferState.h"
#include "stone/Lex/Cursor.h"
#include "stone/Lex/LexerState.h"

#include "llvm/ADT/FunctionExtras.h"
#include "llvm/ADT/SmallVector.h"
#include "llvm/ADT/StringRef.h"
#include <optional>

namespace stone {

class LexerObserver;
using TokenCallback = llvm::function_ref<void(const Token &)>;
using Issues = llvm::SmallVector<diag::DiagID, 8>;
/// Determines the kind of null character encountered.
enum class NullCharacterKind { BufferEnd, Embedded, CodeCompletion };

//===--------------------------------------------------------------------===//
// String Segment Extraction
//===--------------------------------------------------------------------===//

struct StringSegment {
  enum : char { Literal, Expr } Kind;
  SrcLoc Loc;
  unsigned Length, IndentToStrip, CustomDelimiterLen;
  bool IsFirstSegment, IsLastSegment;

  static StringSegment GetLiteral(SrcLoc loc, unsigned length, bool isFirst,
                                  bool isLast, unsigned indentToStrip,
                                  unsigned customDelimiterLen);

  static StringSegment GetExpr(SrcLoc loc, unsigned length);

  SrcLoc GetEndLoc() const;
};

/// \brief The core class responsible for tokenizing `.stone` source files.
class Lexer final {
  SrcUnit &unit;
  SrcMgr &sm;
  Cursor cursor;
  Token nextToken;
  LexerState state;
  LexerObserver *observer = nullptr;
  const char *CodeCompletionPtr = nullptr;

  Lexer(const Lexer &) = delete;
  void operator=(const Lexer &) = delete;

  void Lex();

  void ComputeOffsetsForUnit(SrcUnit &unit);

  void ComputeOffsetsForUnit(SrcUnit &unit, const SrcMgr &sm,
                             const LexerState &begin, const LexerState &end);

  void Init(SrcUnit &unit, SrcMgr &sm);

public:
  explicit Lexer(SrcUnit &unit, SrcMgr &sm);
  explicit Lexer(Lexer &parent, LexerState begin, LexerState end);

  void SetObserver(LexerObserver *observer);
  LexerObserver *GetObserver() const;

  SrcUnit &GetSrcUnit() { return unit; }
  SrcMgr &GetSrcMgr() { return sm; }
  Cursor &GetCursor() { return cursor; }

public:
  void Lex(Token &result);
  void Lex(TokenCallback callback);

  const Token &PeekNext() const { return nextToken; }

  void SeekTo(size_t offset);
  size_t GetOffset() const;
  const char *GetPtrForLoc(SrcLoc loc) const;

  bool IsCodeCompletion() const { return CodeCompletionPtr != nullptr; }
  LexerState GetState() const { return state; }

  //   void Track(diag::DiagID id) { unit.AddDiag(id); }

  //   bool IsKeepingComments() {
  //     return unit.commentRetentionMode != CommentRetentionMode::None;
  //   }
  //   bool IsHashbangAllowed() {
  //     return unit.hashbangMode != HashbangMode::Disallowed;
  //   }
  //   //===--------------------------------------------------------------------===//
  //   // Lexing routines (core forms)
  //   //===--------------------------------------------------------------------===//

  //   void LexHash();
  //   void LexIdentifier();
  //   void LexOperator();
  //   void LexHexNumber();
  //   void LexNumber();
  //   void LexStringLiteral(unsigned customDelimiterLen = 0);
  //   void LexEscapedIdentifier();
  //   void LexRegexLiteral(const char *tokStart);
  //   void TryLexEditorPlaceholder();

  //   //===--------------------------------------------------------------------===//
  //   // Token Formation
  //   //===--------------------------------------------------------------------===//

  //   void FormToken(tok kind, const char *tokStart);
  //   void FormEscapedIdentifierToken(const char *tokStart);
  //   void FormStringLiteralToken(const char *tokStart, bool isMultilineString,
  //                               unsigned customDelimiterLen);

  //   //===--------------------------------------------------------------------===//
  //   // Trivia and Comments
  //   //===--------------------------------------------------------------------===//

  Trivia LexTrivia(const char *allTriviaStart,
                   bool isForTrailingTrivia = false);

  Trivia LexTrivia(Cursor &cursor, bool isForTrailingTrivia = false);

  //   static unsigned LexUnicodeEscape(const char *&curPtr, Issues &issues);

  //   unsigned LexCharacter(const char *&curPtr, char stopQuote,
  //                         bool isMultilineString = false,
  //                         unsigned customDelimiterLen = 0);

  //   /// Advance to the end of the line.
  //   /// If EatNewLine is true, CurPtr will be at end of newline character.
  //   /// Otherwise, CurPtr will be at newline character.
  //   void SkipToEndOfLine(bool EatNewline);

  //   /// Skip a #! hashbang line.
  //   void SkipHashbang(bool EatNewline);

  //   /// Skip to the end of the line of a // comment.
  //   void SkipLineComment(bool EatNewline);

  //   void SkipBlockComment();

  //   bool TryLexConflictMarker(bool eatNewline);
  //   const char *FindEndOfCurlyQuoteStringLiteral(const char *start);

  //   NullCharacterKind GetNullCharacterKind(const char *ptr) const;

  //   static void
  //   GetStringLiteralSegments(const Token &str,
  //                            llvm::SmallVectorImpl<StringSegment> &segments,
  //                            Issues &issues);

  //   //===--------------------------------------------------------------------===//
  //   // Optional Utility Functions
  //   //===--------------------------------------------------------------------===//

  //   static SrcLoc GetSrcLoc(const char *ptr) { return
  //   SrcLoc::GetFromPtr(ptr); }

  //   static llvm::StringRef GetEncodedStringSegmentImpl(
  //       llvm::StringRef str, llvm::SmallVectorImpl<char> &buffer,
  //       bool isFirstSegment, bool isLastSegment, unsigned indentToStrip,
  //       unsigned customDelimiterLen);

  //   static std::optional<size_t> GetCutoffOffset(const char *cutoffPtr,
  //                                                const char *bufferStart) {
  //     if (cutoffPtr) {
  //       return cutoffPtr - bufferStart;
  //     }
  //     return std::nullopt;
  //   }

  //   void CutOffLexing();

  //   bool HasValidBufferBounds() {
  //     return (CurPtr >= BufferStart && CurPtr <= BufferEnd);
  //   }

  //   //===--------------------------------------------------------------------===//
  //   // Static Utilities
  //   //===--------------------------------------------------------------------===//

  //   static Token GetTokenAtLocation(SrcMgr &sm, SrcLoc loc,
  //                                   CommentRetentionMode crm);

  //   static SrcLoc GetLocForStartOfToken(SrcMgr &sm, unsigned bufferID,
  //                                       unsigned offset);
  //   static SrcLoc GetLocForStartOfToken(SrcMgr &sm, SrcLoc loc);
  //   static SrcLoc GetLocForEndOfToken(SrcMgr &sm, SrcLoc loc);
  //   static SrcLoc GetLocForStartOfLine(SrcMgr &sm, SrcLoc loc);
  //   static SrcLoc GetLocForEndOfLine(SrcMgr &sm, SrcLoc loc);

  //   static llvm::StringRef
  //   GetIndentationForLine(SrcMgr &sm, SrcLoc loc,
  //                         llvm::StringRef *extra = nullptr);

  //   static bool IsIdentifier(llvm::StringRef ident);
  //   static tok KindOfIdentifier(llvm::StringRef ident);
  //   static bool IsOperator(llvm::StringRef op);

  //   static void LexUnit(SrcUnit &unit, SrcMgr &sm);

  // private:
  //   static bool IsValidIdentifierContinuationCodePoint(uint32_t c);

  //   static bool AdvanceIf(char const *&ptr, char const *end,
  //                         bool (*predicate)(uint32_t));

  //   static bool AdvanceIfValidStartOfIdentifier(char const *&ptr,
  //                                               char const *end);

  //   static bool AdvanceIfValidContinuationOfIdentifier(char const *&ptr,
  //                                                      char const *end);

  //   static bool AdvanceIfValidStartOfOperator(char const *&ptr, char const
  //   *end);

  //   static bool AdvanceIfValidContinuationOfOperator(char const *&ptr,
  //                                                    char const *end);

private:
};

} // namespace stone

#endif // STONE_LEX_LEXER_H
