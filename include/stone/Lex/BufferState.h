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
  // Raw BufferState Pointers (Physical Range)
  //===--------------------------------------------------------------------===//

  const char *InitialBuffer = nullptr; ///< Pointer to start of buffer.
  const char *MaxBuffer = nullptr;     ///< Pointer to end of buffer.
  const char *EOFBuffer = nullptr;     ///< Optional logical EOF boundary.

  //===--------------------------------------------------------------------===//
  // Lexing Pointers (Current Position State)
  //===--------------------------------------------------------------------===//

  const char *StartPtr = nullptr;       ///< First character of actual content.
  const char *CurPtr = nullptr;         ///< Current lexing position.
  const char *LastCommentPtr = nullptr; ///< Start of last comment (if any).
  const char *CutOffPtr = nullptr;      ///< Optional hard cutoff.

  const char *TmpPtr = nullptr; ///< Holding temporary stuff

  //===--------------------------------------------------------------------===//
  // Offset Metadata (Byte Distances)
  //===--------------------------------------------------------------------===//

  unsigned CurOffset = 0;       ///< Current byte offset (starts at 0).
  unsigned EndOffset = 0;       ///< Artificial EOF byte offset.
  unsigned TotalOffsetSize = 0; ///< Physical size of the buffer.

  //===--------------------------------------------------------------------===//
  // Logical Lexing Bounds (e.g., bookmarks or slices)
  //===--------------------------------------------------------------------===//

  unsigned InitialOffset = 0; ///< Logical lexing start offset.
  unsigned MaxOffset = 0;     ///< Logical lexing end offset.

  //===--------------------------------------------------------------------===//
  // Constructors & Lifecycle
  //===--------------------------------------------------------------------===//

  BufferState() { Clear(); }

  void Init(const char *initialBuffer, size_t bufferSize) {
    InitialBuffer = initialBuffer;
    MaxBuffer = initialBuffer + bufferSize;
    StartPtr = initialBuffer;
    CurPtr = StartPtr;
    TotalOffsetSize = bufferSize;
  }

  bool HasNext() { return (CurPtr < MaxBuffer); }
  char PeekNext() const {
    assert(CurPtr && "Trying to peek null");
    return *CurPtr;
  }

  //===--------------------------------------------------------------------===//
  // Utilities
  //===--------------------------------------------------------------------===//

  /// \returns true if the buffer begins with a UTF-8 BOM marker.
  bool HasUTF8BOM() const {
    return MaxBuffer - InitialBuffer >= 3 &&
           static_cast<unsigned char>(InitialBuffer[0]) == 0xEF &&
           static_cast<unsigned char>(InitialBuffer[1]) == 0xBB &&
           static_cast<unsigned char>(InitialBuffer[2]) == 0xBF;
  }

  /// \brief Clears all fields to a known null state.
  void Clear() {
    InitialBuffer = MaxBuffer = EOFBuffer = StartPtr = CurPtr = nullptr;
    LastCommentPtr = CutOffPtr = nullptr;
    CurOffset = EndOffset = TotalOffsetSize = InitialOffset = MaxOffset = 0;
    ClearTmp();
  }
  void ClearTmp() { TmpPtr = nullptr; }

  /// \brief Asserts internal consistency of buffer layout.
  void Check() const {
    assert(InitialBuffer && MaxBuffer && "BufferState is uninitialized");
    assert(InitialBuffer <= MaxBuffer && "BufferState range is invalid");
    assert(StartPtr >= InitialBuffer && StartPtr <= MaxBuffer &&
           "StartPtr out of range");
    assert(CurPtr >= InitialBuffer && CurPtr <= MaxBuffer &&
           "CurPtr out of range");
    if (EOFBuffer)
      assert(EOFBuffer >= InitialBuffer && EOFBuffer <= MaxBuffer &&
             "EOFBuffer out of range");
  }

  /// \brief Prepares logical lexing bounds from a subrange.
  void ResetForSubrange(const char *begin, const char *end) {
    assert(begin && end && begin <= end && "Invalid subrange");
    StartPtr = begin;
    CurPtr = begin;
    EOFBuffer = end;
    InitialOffset = begin - InitialBuffer;
    MaxOffset = end - InitialBuffer;
  }

  /// \returns true if the current pointer is within the buffer range.
  bool IsValid() const {
    return (CurPtr >= InitialBuffer && CurPtr <= MaxBuffer);
  }

  unsigned GetOffset() const {
    return static_cast<unsigned>(CurPtr - InitialBuffer);
  }
};

} // namespace stone

#endif // STONE_LEX_BUFFERSTATE_H
