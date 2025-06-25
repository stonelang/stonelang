#ifndef STONE_LEX_BUFFERSTATE_H
#define STONE_LEX_BUFFERSTATE_H

namespace stone {

/// \brief Tracks buffer layout, position, and logical slicing for lexing.
/// This structure is internal to the lexer and provides full control over
/// character pointers and their corresponding offsets.
///
/// === Notes ===
/// - "BufferState": denotes a physical range (e.g., [InitialBuffer, MaxBuffer))
/// - "Ptr":     denotes a specific position within that range
/// - Offsets:   mirror positions as byte distances from InitialBuffer
///
struct BufferState final {
  //===--------------------------------------------------------------------===//
  // Core Buffer Pointers
  //===--------------------------------------------------------------------===//
  const char *InitialBuffer = nullptr; ///< Pointer to start of buffer.
  const char *MaxBuffer = nullptr;     ///< Pointer to end of physical buffer.

   ///< Optional logical end-of-buffer limit.
  const char *ArtificialEOF = nullptr;                 
  const char *CurPtr = nullptr; ///< Current scan/lexing position.
  const char *TmpPtr = nullptr; ///< Optional temporary mark (bookmark).

  //===--------------------------------------------------------------------===//
  // Initialization & Reset
  //===--------------------------------------------------------------------===//

  BufferState() = default;

  void Init(const char *buf, size_t size) {
    InitialBuffer = buf;
    MaxBuffer = buf + size;
    CurPtr = buf;
    ArtificialEOF = MaxBuffer;
    TmpPtr = nullptr;
  }

  const char *GetInitialBuffer() const { return InitialBuffer; }
  const char *GetMaxBuffer() const { return MaxBuffer; }
  const char *GetArtificialEOF() const { return ArtificialEOF; }
  const char *GetCurPtr() const { return CurPtr; }

  void ResetForSubrange(const char *begin, const char *end) {
    assert(begin && end && begin <= end && "Invalid subrange");
    assert(begin >= InitialBuffer && end <= MaxBuffer && "Out of range");
    CurPtr = begin;
    ArtificialEOF = end;
  }

  void Clear() {
    InitialBuffer = MaxBuffer = ArtificialEOF = CurPtr = TmpPtr = nullptr;
  }

  bool IsAny(char c) const { return !IsAtEnd() && Peek() == c; }
  bool IsAnyOf(char a, char b) const { return IsAny(a) || IsAny(b); }
  bool IsAnyOf(char a, char b, char c) const {
    return IsAny(a) || IsAny(b) || IsAny(c);
  }

  template <typename... Chars> bool IsAny(Chars... chars) const {
    if (IsAtEnd()) {
      return false;
    }
    char current = Peek();
    return ((current == chars) || ...);
  }

  //===--------------------------------------------------------------------===//
  // Core Lexing Operations
  //===--------------------------------------------------------------------===//

  bool HasNext() const {
    return CurPtr && CurPtr < (ArtificialEOF ? ArtificialEOF : MaxBuffer);
  }

  char Peek() const {
    assert(CurPtr && CurPtr < (ArtificialEOF ? ArtificialEOF : MaxBuffer) &&
           "Peek out of bounds");
    return *CurPtr;
  }

  char Peek(unsigned n = 0) const {
  assert(CurPtr + n < BufferEnd() && "Peek out of range");
  return CurPtr[n];
}


  char LookAhead(size_t lookahead = 1) const {
    const char *ptr = CurPtr + lookahead;
    return (ptr < EOFBuffer) ? *ptr : '\0';
  }

  char LookBehind(unsigned n = 1) const {
    return (CurPtr - n >= InitialBuffer) ? *(CurPtr - n) : '\0';
  }
  bool Cursor::IsNull() const { return CurPtr < EndPtr && *CurPtr == '\0'; }

  void Advance(unsigned n = 1) {
    assert(CurPtr && CurPtr + n <= AtEnd() && "Advance past limit");
    CurPtr += n;
  }

  void Rewind(unsigned n = 1) {
    assert(CurPtr && CurPtr - n >= InitialBuffer &&
           "Rewind before buffer start");
    CurPtr -= n;
  }

  const char *AtEnd() const {
    return ArtificialEOF ? ArtificialEOF : MaxBuffer;
  }

  unsigned GetOffset() const {
    return static_cast<unsigned>(CurPtr - InitialBuffer);
  }

  unsigned GetTotalSize() const {
    return static_cast<unsigned>(MaxBuffer - InitialBuffer);
  }

  unsigned GetRemainingSize() const {
    return static_cast<unsigned>((ArtificialEOF ? ArtificialEOF : MaxBuffer) -
                                 CurPtr);
  }

  llvm::StringRef SliceRemaining() const {
    return llvm::StringRef(CurPtr, GetRemainingSize());
  }

  void JumpToOffset(unsigned offset) {
    assert(InitialBuffer + offset <= MaxBuffer && "Offset out of bounds");
    CurPtr = InitialBuffer + offset;
  }

  void JumpBackFromOffset(unsigned nBytes) {
    assert(CurPtr - nBytes >= InitialBuffer && "JumpBack out of bounds");
    CurPtr -= nBytes;
  }

  bool IsValid() const {
    return CurPtr >= InitialBuffer && CurPtr <= MaxBuffer;
  }

  bool IsAtEnd() const { return !HasNext(); }

  //===--------------------------------------------------------------------===//
  // Temporary Bookmark Utilities
  //===--------------------------------------------------------------------===//

  void MarkTmpPtr() { TmpPtr = CurPtr; }
  void ClearTmpPtr() { TmpPtr = nullptr; }
  const char *GetTmpPtr() const { return TmpPtr; }
  llvm::StringRef GetTmpPtrRange() const {
    assert(TmpPtr && "TmpPtr not marked");
    return llvm::StringRef(TmpPtr, static_cast<size_t>(CurPtr - TmpPtr));
  }

  //===--------------------------------------------------------------------===//
  // Utility Checks
  //===--------------------------------------------------------------------===//

  bool HasUTF8BOM() const {
    return MaxBuffer - InitialBuffer >= 3 &&
           static_cast<unsigned char>(InitialBuffer[0]) == 0xEF &&
           static_cast<unsigned char>(InitialBuffer[1]) == 0xBB &&
           static_cast<unsigned char>(InitialBuffer[2]) == 0xBF;
  }

  void Check() const {
    assert(InitialBuffer && MaxBuffer && "Uninitialized buffer");
    assert(InitialBuffer <= MaxBuffer && "Invalid buffer range");
    assert(CurPtr >= InitialBuffer && CurPtr <= MaxBuffer &&
           "CurPtr out of bounds");
    if (ArtificialEOF) {
      assert(ArtificialEOF >= InitialBuffer && ArtificialEOF <= MaxBuffer &&
             "ArtificialEOF out of bounds");
    }
  }
};

} // namespace stone

#endif // STONE_LEX_BUFFERSTATE_H
