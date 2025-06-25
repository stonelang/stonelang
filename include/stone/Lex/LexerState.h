#ifndef STONE_PARSE_LEXERSTATE_H
#define STONE_PARSE_LEXERSTATE_H

#include "stone/Core/SrcLoc.h"
#include "llvm/ADT/ArrayRef.h"
#include "llvm/ADT/SmallVector.h"

namespace stone {

/// \brief Captures the state of the lexer at a specific point in the source.
///
/// `LexerState` is a snapshot of the current source location and associated
/// leading trivia. It is used to support speculative parsing, rewinding,
/// or lexing embedded/interpolated regions.
class LexerState final {
  SrcLoc loc;                          ///< Source location in the buffer.
  llvm::SmallVector<Trivia, 4> trivia; ///< Leading trivia (optional).

public:
  /// Constructs an invalid state.
  LexerState() = default;

  /// Constructs a valid state with a source location and optional trivia.
  explicit LexerState(SrcLoc loc, llvm::ArrayRef<Trivia> t = {})
      : loc(loc), trivia(t.begin(), t.end()) {}

  /// \returns true if the location is valid.
  bool IsValid() const { return loc.isValid(); }

  /// \returns the stored source location.
  SrcLoc GetLoc() const { return loc; }

  /// Sets the source location.
  void SetLoc(SrcLoc newLoc) { loc = newLoc; }

  /// \returns the associated leading trivia.
  llvm::ArrayRef<Trivia> GetTrivia() const { return trivia; }

  /// Replaces the stored trivia.
  void SetTrivia(llvm::ArrayRef<Trivia> t) {
    trivia.assign(t.begin(), t.end());
  }

  /// Appends a piece of trivia to the end of the list.
  void AddTrivia(const Trivia &t) { trivia.push_back(t); }

  /// Clears all stored trivia.
  void ClearTrivia() { trivia.clear(); }

  /// \returns a new `LexerState` advanced by the given byte offset.
  LexerState Advance(unsigned offset) const {
    assert(IsValid() && "Cannot advance an invalid state");
    return LexerState(loc.getAdvancedLoc(offset), trivia);
  }

  /// Copies the trivia from another state.
  void CopyTriviaFrom(const LexerState &other) { trivia = other.trivia; }
};

} // namespace stone

#endif // STONE_PARSE_LEXERSTATE_H
