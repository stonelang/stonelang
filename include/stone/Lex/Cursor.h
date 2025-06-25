#ifndef STONE_LEX_CURSOR_H
#define STONE_LEX_CURSOR_H

#include "clang/Basic/CharInfo.h"

#include <cassert>

namespace stone {

/// \brief Low-level byte stream navigator over a fixed buffer.
///
/// `Cursor` manages position tracking within a read-only character buffer.
/// It exposes pointer navigation (`CurPtr`), and utility methods like
/// lookahead, skipping, and comment handling.
///
/// === Notes ===
/// - "Buffer": fixed memory range [InitialBuffer, MaxBuffer)
/// - "Ptr":     a current position in the buffer
/// - `EOFBuffer`: allows lexing logical slices
///
class Cursor final {
public:
  //===--------------------------------------------------------------------===//
  // Buffer Bounds (Immutable after construction)
  //===--------------------------------------------------------------------===//

  const char *InitialBuffer = nullptr; ///< Start of memory buffer.
  const char *MaxBuffer = nullptr;     ///< End of buffer (1 past last).
  const char *EOFBuffer = nullptr;     ///< Optional logical cutoff.

  //===--------------------------------------------------------------------===//
  // Navigation State
  //===--------------------------------------------------------------------===//

  const char *CurPtr = nullptr; ///< Current pointer in buffer.

  void Init(const char *buf, size_t size) {
    InitialBuffer = buf;
    MaxBuffer = buf + size;
    CurPtr = buf;
    EOFBuffer = MaxBuffer;
  }

  void Check() const {
    // Basic buffer invariants
    assert(InitialBuffer && "Cursor not initialized");
    assert(MaxBuffer && "MaxBuffer is null");
    assert(InitialBuffer <= MaxBuffer && "Invalid buffer range");

    // Pointer validity
    assert(CurPtr >= InitialBuffer && CurPtr <= MaxBuffer &&
           "CurPtr out of range");
    assert(EOFBuffer >= InitialBuffer && EOFBuffer <= MaxBuffer &&
           "EOFBuffer out of range");

    // BOM check (optional)
    if (HasUTF8BOM()) {
      assert(CurPtr == InitialBuffer && "Unexpected BOM: CurPtr not at start");
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

  char LookAhead(size_t lookahead) const {
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

  const char *GetCurPtr() const { return CurPtr; }

  //===--------------------------------------------------------------------===//
  // Matching & Skipping
  //===--------------------------------------------------------------------===//

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
    while (CurPtr < EOFBuffer && pred(*CurPtr)) {
      ++CurPtr;
    }
  }

  void SkipWhitespace() { SkipWhile(clang::isWhitespace); }

  void SkipUntil(char target) {
    while (!IsAtEnd() && Peek() != target) {
      ++CurPtr;
    }
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
      Retreat(); // Undo
    }
    return false;
  }

  //===--------------------------------------------------------------------===//
  // Utilities
  //===--------------------------------------------------------------------===//

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

  //===--------------------------------------------------------------------===//
  // Character Category Queries
  //===--------------------------------------------------------------------===//

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

  bool IsNewline() const { return IsAnyOf('\n', '\r'); }
  bool IsDigit() const { return !IsAtEnd() && clang::isDigit(Peek()); }

  // bool IsIdentifierHead() const {
  //   return !IsAtEnd() && llvm::isIdentifierHead(Peek());
  // }
  // bool IsIdentifierBody() const {
  //   return !IsAtEnd() && llvm::isIdentifierBody(Peek());
  // }

  bool IsLBracket() const { return Is('['); }
  bool IsRBracket() const { return Is(']'); }
  bool AdvanceIfLBracket() { return AdvanceIf('['); }
  bool AdvanceIfRBracket() { return AdvanceIf(']'); }

  bool IsOpenParen() const { return Is('('); }
  bool IsCloseParen() const { return Is(')'); }
  bool IsOpenBrace() const { return Is('{'); }
  bool IsCloseBrace() const { return Is('}'); }
  bool IsOpenBracket() const { return Is('['); }
  bool IsCloseBracket() const { return Is(']'); }

  // Quotes
  bool IsQuote() const { return IsAny('"', '\''); }
  bool IsBacktick() const { return Is('`'); }

  // Delimiters
  bool IsColon() const { return Is(':'); }
  bool IsSemicolon() const { return Is(';'); }
  bool IsComma() const { return Is(','); }

  // Math / Operators
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

  // Misc
  bool IsDot() const { return Is('.'); }
  bool IsHash() const { return Is('#'); }
  bool IsAtSign() const { return Is('@'); }
  bool IsBackslash() const { return Is('\\'); }
  bool IsUnderscore() const { return Is('_'); }
  bool IsOpenDelimiter(char c) const {
    return c == '(' || c == '[' || c == '{';
  }
  bool IsCloseDelimiter(char c) const {
    return c == ')' || c == ']' || c == '}';
  }
  bool IsBinOpChar() const {
    return IsAny('+', '-', '*', '/', '%', '&', '|', '^');
  }
  bool IsBracket() const { return IsAny('(', ')', '[', ']', '{', '}'); }
  bool IsDelimiter() const { return IsAny(',', ';', ':'); }
};

} // namespace stone

#endif // STONE_LEX_CURSOR_H
