#ifndef STONE_LEX_SCANNER_H
#define STONE_LEX_SCANNER_H

#include "clang/Basic/CharInfo.h"
#include <cassert>

namespace stone {

/// \brief Low-level byte stream navigator over a fixed buffer.
///
/// `Scanner` manages position tracking within a read-only character buffer.
/// It exposes current character (`CurChar`), pointer navigation (`CurPtr`),
/// and utility methods like lookahead, skipping, and comment handling.
///
/// === Notes ===
/// - "Buffer": fixed memory range [InitialBuffer, MaxBuffer)
/// - "Ptr":     a current position in the buffer
/// - `EOFBuffer`: allows lexing logical slices
///
class Scanner final {
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
  char CurChar = '\0';          ///< Cached current character.

  //===--------------------------------------------------------------------===//
  // Constructors
  //===--------------------------------------------------------------------===//

  void Init(const char *buf, size_t size) {
    InitialBuffer = buf;
    MaxBuffer = buf + size;
    CurPtr = buf;
    CurChar = (CurPtr < MaxBuffer) ? *CurPtr : '\0';
    EOFBuffer = MaxBuffer;
  }

  //===--------------------------------------------------------------------===//
  // Core Navigation
  //===--------------------------------------------------------------------===//

  void Advance() {
    if (CurPtr < EOFBuffer)
      ++CurPtr;
    UpdateCurChar();
  }

  void Back() {
    if (CurPtr > InitialBuffer) {
      --CurPtr;
      CurChar = *CurPtr;
    }
  }

  void Next(char &result) {
    result = CurChar;
    Advance();
  }

  char Peek() const { return CurChar; }

  char LookAhead(size_t lookahead) const {
    const char *ptr = CurPtr + lookahead;
    return (ptr < EOFBuffer) ? *ptr : '\0';
  }

  /// \returns the character `n` steps behind, or `\0` if before InitialBuffer.
  char LookBehind(unsigned n = 1) const {
    if (CurPtr - n >= InitialBuffer) {
      return *(CurPtr - n);
    }
    return '\0';
  }

  bool HasNext() const { return CurPtr < EOFBuffer; }

  bool IsAtEnd() const { return CurPtr >= EOFBuffer; }

  /// \returns true if the current pointer is at the given character.
  bool IsAt(char c) const { return !IsAtEnd() && CurChar == c; }

  void Reset(const char *ptr) {
    assert(ptr >= InitialBuffer && ptr <= MaxBuffer && "Reset out of range");
    CurPtr = ptr;
    UpdateCurChar();
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
    if (!IsAtEnd() && *CurPtr == expected) {
      ++CurPtr;
      UpdateCurChar();
      return true;
    }
    return false;
  }

  bool Expect(char expected) const {
    return (!IsAtEnd() && *CurPtr == expected);
  }

  template <typename Pred> void SkipWhile(Pred pred) {
    while (CurPtr < EOFBuffer && pred(*CurPtr)) {
      ++CurPtr;
    }
    UpdateCurChar();
  }

  void SkipWhitespace() { SkipWhile(clang::isWhitespace); }

  void SkipUntil(char target) {
    while (!IsAtEnd() && *CurPtr != target) {
      ++CurPtr;
    }
    UpdateCurChar();
  }

  void SkipLineComment() {
    while (!IsAtEnd()) {
      char c = *CurPtr++;
      if (c == '\n' || c == '\r')
        break;
    }
    UpdateCurChar();
  }

  void SkipBlockComment() {
    while (!IsAtEnd()) {
      char c = *CurPtr++;
      if (c == '*' && !IsAtEnd() && *CurPtr == '/') {
        ++CurPtr;
        break;
      }
    }
    UpdateCurChar();
  }

  bool SkipComment() {
    if (AdvanceIf('/') && !IsAtEnd()) {
      if (*CurPtr == '/') {
        ++CurPtr;
        SkipLineComment();
        return true;
      }
      if (*CurPtr == '*') {
        ++CurPtr;
        SkipBlockComment();
        return true;
      }
      --CurPtr; // Undo
      UpdateCurChar();
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
    UpdateCurChar();
  }

  void Check() const {
    assert(InitialBuffer && MaxBuffer && "Uninitialized scanner");
    assert(InitialBuffer <= MaxBuffer);
    assert(CurPtr >= InitialBuffer && CurPtr <= MaxBuffer);
    assert(EOFBuffer >= InitialBuffer && EOFBuffer <= MaxBuffer);
  }

private:
  void UpdateCurChar() { CurChar = (CurPtr < EOFBuffer) ? *CurPtr : '\0'; }
};

} // namespace stone

#endif // STONE_LEX_SCANNER_H
