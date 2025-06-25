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
  const char *ArtificalEOF = nullptr; ///< Optional logical end-of-buffer limit.
  const char *CurPtr = nullptr;       ///< Current scan/lexing position.
  const char *TmpPtr = nullptr;       ///< Optional temporary mark (bookmark).

  //===--------------------------------------------------------------------===//
  // Initialization & Reset
  //===--------------------------------------------------------------------===//

  BufferState() = default;

  void Init(const char *buf, size_t size) {
    InitialBuffer = buf;
    MaxBuffer = buf + size;
    CurPtr = buf;
    ArtificalEOF = MaxBuffer;
    TmpPtr = nullptr;
  }
  
  const char *GetInitialBuffer() const { return InitialBuffer; }
  const char *GetMaxBuffer() const { return MaxBuffer; }
  const char *GetArtificalEOF() const { return ArtificalEOF; }
  const char *GetCurPtr() const { return CurPtr; }

  void ResetForSubrange(const char *begin, const char *end) {
    assert(begin && end && begin <= end && "Invalid subrange");
    assert(begin >= InitialBuffer && end <= MaxBuffer && "Out of range");
    CurPtr = begin;
    ArtificalEOF = end;
  }

  void Clear() {
    InitialBuffer = MaxBuffer = ArtificalEOF = CurPtr = TmpPtr = nullptr;
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

  [[nodiscard]] bool HasNext() const {
    return CurPtr && CurPtr < (ArtificalEOF ? ArtificalEOF : MaxBuffer);
  }

  [[nodiscard]] char Peek() const {
    assert(CurPtr && CurPtr < (ArtificalEOF ? ArtificalEOF : MaxBuffer) &&
           "Peek out of bounds");
    return *CurPtr;
  }

  void Advance(unsigned n = 1) {
    assert(CurPtr && CurPtr + n <= (ArtificalEOF ? ArtificalEOF : MaxBuffer) &&
           "Advance past limit");
    CurPtr += n;
  }

  void Rewind(unsigned n = 1) {
    assert(CurPtr && CurPtr - n >= InitialBuffer &&
           "Rewind before buffer start");
    CurPtr -= n;
  }

  [[nodiscard]] unsigned GetOffset() const {
    return static_cast<unsigned>(CurPtr - InitialBuffer);
  }

  [[nodiscard]] unsigned GetTotalSize() const {
    return static_cast<unsigned>(MaxBuffer - InitialBuffer);

    [[nodiscard]] unsigned GetRemainingSize() const {
      return static_cast<unsigned>((ArtificalEOF ? ArtificalEOF : MaxBuffer) -
                                   CurPtr);
    }

    [[nodiscard]] llvm::StringRef SliceRemaining() const {
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
    [[nodiscard]] unsigned GetTotalSize() const {
      return static_cast<unsigned>(MaxBuffer - InitialBuffer);
    }

    [[nodiscard]] bool IsValid() const {
      return CurPtr >= InitialBuffer && CurPtr <= MaxBuffer;
    }
    //===--------------------------------------------------------------------===//
    // Temporary Bookmark Utilities
    //===--------------------------------------------------------------------===//
    void MarkTmpPtr() { TmpPtr = CurPtr; }
    void ClearTmpPtr() { TmpPtr = nullptr; }

    [[nodiscard]] const char *GetTmpPtr() const { return TmpPtr; }

    [[nodiscard]] llvm::StringRef GetTmpPtrRange() const {
      assert(TmpPtr && "TmpPtr not marked");
      return llvm::StringRef(TmpPtr, static_cast<size_t>(CurPtr - TmpPtr));
    }

    //===--------------------------------------------------------------------===//
    // Utility Checks
    //===--------------------------------------------------------------------===//
    [[nodiscard]] bool HasUTF8BOM() const {
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
      if (ArtificalEOF) {
        assert(ArtificalEOF >= InitialBuffer && ArtificalEOF <= MaxBuffer &&
               "ArtificalEOF out of bounds");
      }
    }
  };
} // namespace stone

#endif // STONE_LEX_BUFFERSTATE_H
