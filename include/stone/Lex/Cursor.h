#ifndef STONE_LEX_CURSOR_H
#define STONE_LEX_CURSOR_H

#include "clang/Basic/CharInfo.h"

#include <cassert>

namespace stone {

/// Inline helpers for identifier classification
inline bool IsStoneIdentHead(char c) {
  return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c == '_');
}

inline bool IsStoneIdentBody(char c) {
  return IsStoneIdentHead(c) || (c >= '0' && c <= '9');
}

class Cursor;
class CursorRange {
  const char *start = nullptr;
  const char *end = nullptr;

public:
  explicit CursorRange(const char *start) : start(start), end(nullptr) {}

  void Finish(const Cursor &cursor);

  const char *Begin() const { return start; }
  const char *End() const { return end; }

  llvm::StringRef AsStringRef() const {
    return llvm::StringRef(start, static_cast<size_t>(end - start));
  }

  bool IsValid() const { return start && end && start <= end; }
};

class Cursor final {
  const char *TmpPtr = nullptr;

public:
  const char *InitialBuffer = nullptr;
  const char *MaxBuffer = nullptr;
  const char *EOFBuffer = nullptr;
  const char *CurPtr = nullptr;

  void Init(const char *buf, size_t size) {
    InitialBuffer = buf;
    MaxBuffer = buf + size;
    CurPtr = buf;
    EOFBuffer = MaxBuffer;
  }

  void Clear() {
    InitialBuffer = nullptr;
    MaxBuffer = nullptr;
    EOFBuffer = nullptr;
    CurPtr = nullptr;
  }

  void Check() const {
    assert(InitialBuffer && "Cursor not initialized");
    assert(MaxBuffer && "MaxBuffer is null");
    assert(InitialBuffer <= MaxBuffer && "Invalid buffer range");
    assert(CurPtr >= InitialBuffer && CurPtr <= MaxBuffer &&
           "CurPtr out of range");
    assert(EOFBuffer >= InitialBuffer && EOFBuffer <= MaxBuffer &&
           "EOFBuffer out of range");

    if (HasUTF8BOM()) {
      assert(CurPtr == InitialBuffer && "Unexpected BOM: CurPtr not at start");
    }
  }

  const char *GetCurPtr() const { return CurPtr; }
  const char *GetInitialBuffer() const { return InitialBuffer; }
  const char *GetMaxBuffer() const { return MaxBuffer; }
  const char *GetEOFBuffer() const { return EOFBuffer; }

  void MarkTmpPtr() { TmpPtr = CurPtr; }
  void ClearTmpPtr() { TmpPtr = nullptr; }
  const char *GetTmpPtr() const { return TmpPtr; }
  llvm::StringRef GetTmpPtrRange() const {
    return llvm::StringRef(TmpPtr, static_cast<size_t>(CurPtr - TmpPtr));
  }

  size_t TmpLength() const {
    assert(TmpPtr && "Tmp not marked");
    return static_cast<size_t>(CurPtr - TmpPtr);
  }

  llvm::StringRef GetTmpRange() const {
    return llvm::StringRef(TmpPtr, TmpLength());
  }

  void SkipUTF8BOM() {
    if (HasUTF8BOM()) {
      Advance();
      Advance();
      Advance();
    }
  }

  void Advance() {
    if (CurPtr < EOFBuffer)
      ++CurPtr;
  }

  void Retreat() {
    if (CurPtr > InitialBuffer)
      --CurPtr;
  }

  void Next(char &result) {
    result = Peek();
    Advance();
  }

  char Peek() const {
    assert(CurPtr && "Trying to peek null");
    return *CurPtr;
  }

  char LookAhead(size_t lookahead = 1) const {
    const char *ptr = CurPtr + lookahead;
    return (ptr < EOFBuffer) ? *ptr : '\0';
  }

  char LookBehind(unsigned n = 1) const {
    return (CurPtr - n >= InitialBuffer) ? *(CurPtr - n) : '\0';
  }

  bool HasNext() const { return CurPtr < EOFBuffer; }
  bool IsAtEnd() const { return CurPtr >= EOFBuffer; }
  bool IsAt(char c) const { return !IsAtEnd() && Peek() == c; }

  void Reset(const char *ptr) {
    assert(ptr >= InitialBuffer && ptr <= MaxBuffer && "Reset out of range");
    CurPtr = ptr;
  }

  void SetLogicalEOF(const char *eof) {
    assert(eof >= InitialBuffer && eof <= MaxBuffer && "EOF out of range");
    EOFBuffer = eof;
  }

  unsigned GetOffset() const {
    return static_cast<unsigned>(CurPtr - InitialBuffer);
  }

  bool AdvanceIf(char expected) {
    if (Is(expected)) {
      ++CurPtr;
      return true;
    }
    return false;
  }

  template <typename... Chars> bool AdvanceIfAny(Chars... chars) {
    if (IsAny(chars...)) {
      Advance();
      return true;
    }
    return false;
  }

  bool Expect(char expected) const { return Is(expected); }

  template <typename Pred> void SkipWhile(Pred pred) {
    while (CurPtr < EOFBuffer && pred(*CurPtr))
      ++CurPtr;
  }

  void SkipWhitespace() { SkipWhile(clang::isWhitespace); }

  void SkipUntil(char target) {
    while (!IsAtEnd() && Peek() != target)
      ++CurPtr;
  }

  void SkipLineComment() {
    while (!IsAtEnd()) {
      char c = *CurPtr++;
      if (c == '\n' || c == '\r')
        break;
    }
  }

  void SkipBlockComment() {
    while (!IsAtEnd()) {
      char c = *CurPtr++;
      if (c == '*' && !IsAtEnd() && Peek() == '/') {
        ++CurPtr;
        break;
      }
    }
  }

  bool SkipComment() {
    if (AdvanceIf('/') && !IsAtEnd()) {
      if (Peek() == '/') {
        ++CurPtr;
        SkipLineComment();
        return true;
      }
      if (Peek() == '*') {
        ++CurPtr;
        SkipBlockComment();
        return true;
      }
      Retreat();
    }
    return false;
  }

  bool HasUTF8BOM() const {
    return (MaxBuffer - InitialBuffer >= 3) &&
           static_cast<unsigned char>(InitialBuffer[0]) == 0xEF &&
           static_cast<unsigned char>(InitialBuffer[1]) == 0xBB &&
           static_cast<unsigned char>(InitialBuffer[2]) == 0xBF;
  }

  void ResetRange(const char *begin, const char *end) {
    assert(begin && end && begin <= end && "Invalid scanner subrange");
    CurPtr = begin;
    EOFBuffer = end;
  }

  CursorRange Mark() const { return CursorRange(CurPtr); }

  // Character predicates

  bool Is(char c) const { return !IsAtEnd() && Peek() == c; }
  bool IsAny(char c) const { return Is(c); }
  bool IsAnyOf(char a, char b) const { return Is(a) || Is(b); }
  bool IsAnyOf(char a, char b, char c) const { return Is(a) || Is(b) || Is(c); }

  template <typename... Chars> bool IsAny(Chars... chars) const {
    if (IsAtEnd())
      return false;
    char current = Peek();
    return ((current == chars) || ...);
  }

  // Whitespace and line breaks

  bool IsSpace() const { return Is(' '); }
  bool IsTab() const { return Is('\t'); }
  bool IsVerticalTab() const { return Is('\v'); }
  bool IsFormFeed() const { return Is('\f'); }
  bool IsCarriageReturn() const { return Is('\r'); }
  bool IsNewline() const { return IsAnyOf('\n', '\r'); }

  bool IsDigit() const { return !IsAtEnd() && clang::isDigit(Peek()); }

  bool IsHorizontalWhitespace() const {
    return !IsAtEnd() && (Peek() == ' ' || Peek() == '\t');
  }

  bool IsVerticalWhitespace() const {
    return !IsAtEnd() && (Peek() == '\n' || Peek() == '\r');
  }

  bool IsWhitespace() const {
    return IsHorizontalWhitespace() || IsVerticalWhitespace();
  }

  bool IsLineBreakStart() const { return Peek() == '\n' || Peek() == '\r'; }

  // Brackets and delimiters

  bool IsOpeningDelimiter() const { return IsAny('(', '{', '['); }
  bool IsClosingDelimiter() const { return IsAny(')', '}', ']'); }
  bool IsBracket() const { return IsAny('(', ')', '[', ']', '{', '}'); }

  bool IsDelimiter() const {
    return IsAny('@', '{', '[', '(', '}', ']', ')', ',', ';', ':', '\\');
  }

  // Operators and symbols

  bool IsOp() const {
    return IsAny('%', '>', '=', '-', '+', '*', '&', '|', '^', '~', '.');
  }

  bool IsPunctuator() const {
    return !IsAtEnd() && clang::isPunctuation(Peek());
  }

  bool IsArithmeticOp() const { return IsAny('+', '-', '*', '/', '%'); }
  bool IsBitwiseOp() const { return IsAny('&', '|', '^', '~'); }
  bool IsAssignmentOp() const { return Is('='); }
  bool IsComparisonOp() const { return IsAny('>', '<'); }
  bool IsLogicalOp() const { return IsAny('&', '|', '!'); }

  // Single-symbol checks

  bool IsLBracket() const { return Is('['); }
  bool IsRBracket() const { return Is(']'); }
  bool IsOpenParen() const { return Is('('); }
  bool IsCloseParen() const { return Is(')'); }
  bool IsOpenBrace() const { return Is('{'); }
  bool IsCloseBrace() const { return Is('}'); }
  bool IsQuote() const { return IsAny('"', '\''); }
  bool IsBacktick() const { return Is('`'); }
  bool IsColon() const { return Is(':'); }
  bool IsSemicolon() const { return Is(';'); }
  bool IsComma() const { return Is(','); }
  bool IsPlus() const { return Is('+'); }
  bool IsMinus() const { return Is('-'); }
  bool IsStar() const { return Is('*'); }
  bool IsSlash() const { return Is('/'); }
  bool IsPercent() const { return Is('%'); }
  bool IsAmpersand() const { return Is('&'); }
  bool IsPipe() const { return Is('|'); }
  bool IsCaret() const { return Is('^'); }
  bool IsEqual() const { return Is('='); }
  bool IsBang() const { return Is('!'); }
  bool IsLess() const { return Is('<'); }
  bool IsGreater() const { return Is('>'); }
  bool IsDot() const { return Is('.'); }
  bool IsHash() const { return Is('#'); }
  bool IsAtSign() const { return Is('@'); }
  bool IsBackslash() const { return Is('\\'); }
  bool IsUnderscore() const { return Is('_'); }

  // Identifier classification

  bool IsIdentHead() const { return !IsAtEnd() && IsStoneIdentHead(Peek()); }

  bool IsIdentBody() const { return !IsAtEnd() && IsStoneIdentBody(Peek()); }

  bool IsIdent() const { return IsIdentHead(); }
};

} // namespace stone

#endif // STONE_LEX_CURSOR_H
