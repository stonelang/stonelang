#ifndef STONE_PARSE_LEXERBOOKMARK_H
#define STONE_PARSE_LEXERBOOKMARK_H

#include "stone/Core/SrcLoc.h"
#include "llvm/ADT/StringRef.h"

namespace stone {

/// \brief Captures a fixed position in the source buffer for speculative
/// parsing.
///
/// `LexerBookmark` is a lightweight snapshot of the current lexical position
/// and associated trivia (e.g., whitespace or comments). It allows the parser
/// to backtrack or resume parsing from a specific point without side effects.
///
/// Typical use cases:
/// - Speculative parsing (e.g., disambiguation)
/// - Rewinding after a parse failure
/// - Lexing embedded/interpolated syntactic regions
/// - Saving/restoring trivia position across token boundaries
class LexerBookmark final {
  ///< Current lexical position in the source.
  SrcLoc loc;
  ///< Leading trivia before the associated token.
  llvm::StringRef leadingTrivia;

public:
  /// \brief Constructs an invalid bookmark (useful as a placeholder).
  LexerBookmark() = default;

  /// \brief Constructs a bookmark at the given location with optional trivia.
  ///
  /// \param loc Lexical source location.
  /// \param trivia Leading trivia (e.g., whitespace/comments).
  explicit LexerBookmark(SrcLoc loc, llvm::StringRef trivia = {})
      : loc(loc), leadingTrivia(trivia) {}

  /// \returns true if this bookmark is valid (i.e., has a real source
  /// location).
  bool IsValid() const { return loc.isValid(); }

  /// \returns the stored source location.
  SrcLoc GetLoc() const { return loc; }

  /// \returns any leading trivia associated with this bookmark.
  llvm::StringRef GetLeadingTrivia() const { return leadingTrivia; }

  /// \brief Advances the bookmark's location by the given offset (in bytes).
  ///
  /// This creates a shallow copy of the current bookmark, with the new
  /// location offset forward.
  ///
  /// \param offset Byte offset to move forward in the source.
  /// \returns A new bookmark at the advanced location.
  LexerBookmark Advance(unsigned offset) const {
    assert(IsValid() && "Attempting to advance an invalid bookmark");
    return LexerBookmark(loc.getAdvancedLoc(offset), leadingTrivia);
  }
};

} // namespace stone

#endif // STONE_PARSE_LEXERBOOKMARK_H
