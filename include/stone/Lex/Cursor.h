#ifndef STONE_LEX_BUFFERSTATE_H
#define STONE_LEX_BUFFERSTATE_H

#include "clang/Basic/CharInfo.h"
#include "llvm/ADT/StringRef.h"
#include <cassert>

namespace stone {

/// Returns true if the character is a valid start for a Stone identifier.
inline bool IsStoneIdentHead(char c) {
  return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c == '_');
}

/// Returns true if the character is valid in the body of a Stone identifier.
inline bool IsStoneIdentBody(char c) {
  return IsStoneIdentHead(c) || (c >= '0' && c <= '9');
}

/// Tracks a character range in the buffer.
class BufferRange {
  const char *start = nullptr;
  const char *end = nullptr;

public:
  explicit BufferRange(const char *start) : start(start), end(nullptr) {}

  void Finish(const class BufferState &state);

  [[nodiscard]] const char *Begin() const { return start; }
  [[nodiscard]] const char *End() const { return end; }

  [[nodiscard]] llvm::StringRef AsStringRef() const {
    return llvm::StringRef(start, static_cast<size_t>(end - start));
  }

  [[nodiscard]] bool IsValid() const { return start && end && start <= end; }
};

/// Manages and tracks the current position and limits in a source buffer.
class BufferState final {
  const char *TmpPtr = nullptr;

public:
  const char *InitialBuffer = nullptr;
  const char *MaxBuffer = nullptr;
  const char *EOFBuffer = nullptr;
  const char *CurPtr = nullptr;

  // Initialize with raw buffer
  void Init(const char *buf, size_t size) {
    InitialBuffer = buf;
    MaxBuffer = buf + size;
    CurPtr = buf;
    EOFBuffer = MaxBuffer;
  }

  void Clear() {
    InitialBuffer = MaxBuffer = EOFBuffer = CurPtr = nullptr;
  }

  void Check() const {
    assert(InitialBuffer && "BufferState not initialized");
    assert(MaxBuffer && InitialBuffer <= MaxBuffer);
    assert(CurPtr >= InitialBuffer && CurPtr <= MaxBuffer);
    assert(EOFBuffer >= InitialBuffer && EOFBuffer <= MaxBuffer);

    if (HasUTF8BOM()) {
      assert(CurPtr == InitialBuffer && "Unexpected BOM");
    }
  }

  // Getters
  [[nodiscard]] const char *GetCurPtr() const { return CurPtr; }
  [[nodiscard]] const char *GetInitialBuffer() const { return InitialBuffer; }
  [[nodiscard]] const char *GetMaxBuffer() const { return MaxBuffer; }
  [[nodiscard]] const char *GetEOFBuffer() const { return EOFBuffer; }

  // Temporary marker (used in lexing tokens)
  void MarkTmpPtr() { TmpPtr = CurPtr; }
  void ClearTmpPtr() { TmpPtr = nullptr; }

  [[nodiscard]] const char *GetTmpPtr() const { return TmpPtr; }
  [[nodiscard]] llvm::StringRef GetTmpPtrRange() const {
    return llvm::StringRef(TmpPtr, static_cast<size_t>(CurPtr - TmpPtr));
  }

  [[nodiscard]] size_t TmpLength() const {
    assert(TmpPtr && "TmpPtr not set");
    return static_cast<size_t>(CurPtr - TmpPtr);
  }

  [[nodiscard]] llvm::StringRef GetTmpRange() const {
    return llvm::StringRef(TmpPtr, TmpLength());
  }

  // Advance & retreat by one
  void Advance() {
    if (CurPtr < EOFBuffer)
      ++CurPtr;
  }

  void Retreat() {
    if (CurPtr > InitialBuffer)
      --CurPtr;
  }

  // Next character
  void Next(char &result) {
    result = Peek();
    Advance();
  }

  // Character access
  [[nodiscard]] char Peek() const {
    assert(CurPtr && "Peek called on null pointer");
    return *CurPtr;
  }

  [[nodiscard]] char LookAhead(size_t n = 1) const {
    const char *ptr = CurPtr + n;
    return (ptr < EOFBuffer) ? *ptr : '\0';
  }

  [[nodiscard]] char LookBehind(unsigned n = 1) const {
    return (CurPtr - n >= InitialBuffer) ? *(CurPtr - n) : '\0';
  }

  // Buffer navigation
  void Reset(const char *ptr) {
    assert(ptr >= InitialBuffer && ptr <= MaxBuffer && "Pointer out of bounds");
    CurPtr = ptr;
  }

  void SetLogicalEOF(const char *eof) {
    assert(eof >= InitialBuffer && eof <= MaxBuffer && "EOF out of bounds");
    EOFBuffer = eof;
  }

  [[nodiscard]] unsigned GetOffset() const {
    return static_cast<unsigned>(CurPtr - InitialBuffer);
  }

  // Advance if current char matches expected
  bool AdvanceIf(char expected) {
    if (Is(expected)) {
      ++CurPtr;
      return true;
    }
    return false;
  }

  // Advance if any of the chars match
  template <typename... Chars>
  bool AdvanceIfAny(Chars... chars) {
    if (IsAny(chars...)) {
      Advance();
      return true;
    }
    return false;
  }

  // Scanning helpers
  template <typename Pred>
  void SkipWhile(Pred pred) {
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

  void SkipUTF8BOM() {
    if (HasUTF8BOM()) {
      Advance(); Advance(); Advance();
    }
  }

  [[nodiscard]] bool HasUTF8BOM() const {
    return (MaxBuffer - InitialBuffer >= 3) &&
           static_cast<unsigned char>(InitialBuffer[0]) == 0xEF &&
           static_cast<unsigned char>(InitialBuffer[1]) == 0xBB &&
           static_cast<unsigned char>(InitialBuffer[2]) == 0xBF;
  }

  void ResetRange(const char *begin, const char *end) {
    assert(begin && end && begin <= end);
    CurPtr = begin;
    EOFBuffer = end;
  }

  [[nodiscard]] BufferRange Mark() const { return BufferRange(CurPtr); }

  // Position checks
  [[nodiscard]] bool HasNext() const { return CurPtr < EOFBuffer; }
  [[nodiscard]] bool IsAtEnd() const { return CurPtr >= EOFBuffer; }
  [[nodiscard]] bool IsAt(char c) const { return !IsAtEnd() && Peek() == c; }

  // Character matchers
  [[nodiscard]] bool Is(char c) const { return !IsAtEnd() && Peek() == c; }

  [[nodiscard]] bool IsAny(char c) const { return Is(c); }

  [[nodiscard]] bool IsAnyOf(char a, char b) const {
    return Is(a) || Is(b);
  }

  [[nodiscard]] bool IsAnyOf(char a, char b, char c) const {
    return Is(a) || Is(b) || Is(c);
  }

  template <typename... Chars>
  [[nodiscard]] bool IsAny(Chars... chars) const {
    if (IsAtEnd()) return false;
    char current = Peek();
    return ((current == chars) || ...);
  }

  // Whitespace and line breaks
  [[nodiscard]] bool IsWhitespace() const {
    return Is(' ') || Is('\t') || Is('\n') || Is('\r');
  }

  [[nodiscard]] bool IsLineBreakStart() const {
    return Peek() == '\n' || Peek() == '\r';
  }

  // Delimiters
  [[nodiscard]] bool IsOpeningDelimiter() const { return IsAny('(', '{', '['); }
  [[nodiscard]] bool IsClosingDelimiter() const { return IsAny(')', '}', ']'); }
  [[nodiscard]] bool IsBracket() const { return IsAny('(', ')', '[', ']', '{', '}'); }

  // Operators and symbols
  [[nodiscard]] bool IsOp() const {
    return IsAny('%', '>', '=', '-', '+', '*', '&', '|', '^', '~', '.');
  }

  [[nodiscard]] bool IsDigit() const {
    return !IsAtEnd() && clang::isDigit(Peek());
  }

  //===----------------------------------------------------------------------===//
// Character Classification
//===----------------------------------------------------------------------===//

[[nodiscard]] bool IsSpace() const { return Is(' '); }
[[nodiscard]] bool IsTab() const { return Is('\t'); }
[[nodiscard]] bool IsVerticalTab() const { return Is('\v'); }
[[nodiscard]] bool IsFormFeed() const { return Is('\f'); }
[[nodiscard]] bool IsCarriageReturn() const { return Is('\r'); }
[[nodiscard]] bool IsNewline() const { return IsAnyOf('\n', '\r'); }

[[nodiscard]] bool IsDigit() const {
  return !IsAtEnd() && clang::isDigit(Peek());
}

[[nodiscard]] bool IsHorizontalWhitespace() const {
  return !IsAtEnd() && (Peek() == ' ' || Peek() == '\t');
}

[[nodiscard]] bool IsVerticalWhitespace() const {
  return !IsAtEnd() && (Peek() == '\n' || Peek() == '\r');
}

[[nodiscard]] bool IsWhitespace() const {
  return IsHorizontalWhitespace() || IsVerticalWhitespace();
}

[[nodiscard]] bool IsLineBreakStart() const {
  return Peek() == '\n' || Peek() == '\r';
}

// Brackets and Delimiters

[[nodiscard]] bool IsOpeningDelimiter() const { return IsAny('(', '{', '['); }
[[nodiscard]] bool IsClosingDelimiter() const { return IsAny(')', '}', ']'); }
[[nodiscard]] bool IsBracket() const { return IsAny('(', ')', '[', ']', '{', '}'); }

[[nodiscard]] bool IsDelimiter() const {
  return IsAny('@', '{', '[', '(', '}', ']', ')', ',', ';', ':', '\\');
}

// Operators and Symbols

[[nodiscard]] bool IsOp() const {
  return IsAny('%', '>', '=', '-', '+', '*', '&', '|', '^', '~', '.');
}

[[nodiscard]] bool IsPunctuator() const {
  return !IsAtEnd() && clang::isPunctuation(Peek());
}

[[nodiscard]] bool IsArithmeticOp() const { return IsAny('+', '-', '*', '/', '%'); }
[[nodiscard]] bool IsBitwiseOp() const { return IsAny('&', '|', '^', '~'); }
[[nodiscard]] bool IsAssignmentOp() const { return Is('='); }
[[nodiscard]] bool IsComparisonOp() const { return IsAny('>', '<'); }
[[nodiscard]] bool IsLogicalOp() const { return IsAny('&', '|', '!'); }

// Single-symbol Checks

[[nodiscard]] bool IsLBracket() const { return Is('['); }
[[nodiscard]] bool IsRBracket() const { return Is(']'); }
[[nodiscard]] bool IsOpenParen() const { return Is('('); }
[[nodiscard]] bool IsCloseParen() const { return Is(')'); }
[[nodiscard]] bool IsOpenBrace() const { return Is('{'); }
[[nodiscard]] bool IsCloseBrace() const { return Is('}'); }
[[nodiscard]] bool IsQuote() const { return IsAny('"', '\''); }
[[nodiscard]] bool IsBacktick() const { return Is('`'); }
[[nodiscard]] bool IsColon() const { return Is(':'); }
[[nodiscard]] bool IsSemicolon() const { return Is(';'); }
[[nodiscard]] bool IsComma() const { return Is(','); }
[[nodiscard]] bool IsPlus() const { return Is('+'); }
[[nodiscard]] bool IsMinus() const { return Is('-'); }
[[nodiscard]] bool IsStar() const { return Is('*'); }
[[nodiscard]] bool IsSlash() const { return Is('/'); }
[[nodiscard]] bool IsPercent() const { return Is('%'); }
[[nodiscard]] bool IsAmpersand() const { return Is('&'); }
[[nodiscard]] bool IsPipe() const { return Is('|'); }
[[nodiscard]] bool IsCaret() const { return Is('^'); }
[[nodiscard]] bool IsEqual() const { return Is('='); }
[[nodiscard]] bool IsBang() const { return Is('!'); }
[[nodiscard]] bool IsLess() const { return Is('<'); }
[[nodiscard]] bool IsGreater() const { return Is('>'); }
[[nodiscard]] bool IsDot() const { return Is('.'); }
[[nodiscard]] bool IsHash() const { return Is('#'); }
[[nodiscard]] bool IsAtSign() const { return Is('@'); }
[[nodiscard]] bool IsBackslash() const { return Is('\\'); }
[[nodiscard]] bool IsUnderscore() const { return Is('_'); }

// Identifier Classification

[[nodiscard]] bool IsIdentHead() const {
  return !IsAtEnd() && IsStoneIdentHead(Peek());
}

[[nodiscard]] bool IsIdentBody() const {
  return !IsAtEnd() && IsStoneIdentBody(Peek());
}

[[nodiscard]] bool IsIdent() const {
  return IsIdentHead();
}
};

inline void BufferRange::Finish(const BufferState &state) {
  end = state.GetCurPtr();
}

} // namespace stone

#endif // STONE_LEX_BUFFERSTATE_H
