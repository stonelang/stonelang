//===----------------------------------------------------------------------===//
// Lexer.h - Source File Lexer Interface
//
// This file defines the `Lexer` class, responsible for tokenizing `.stone`
// source units. It supports both pull-based (parser-driven) and push-based
// (streaming) tokenization models.
//
// The lexer supports trivia tracking, hashbang skipping, conflict marker
// detection, and optional observation via `LexerObserver`.
//===----------------------------------------------------------------------===//

#ifndef STONE_LEX_LEXER_H
#define STONE_LEX_LEXER_H

#include "stone/Core/SrcUnit.h"
#include "stone/Core/SrcMgr.h"
#include "stone/Diag/DiagID.h"
#include "llvm/ADT/SmallVector.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/ADT/FunctionExtras.h"

namespace stone {

class LexerObserver;

/// \brief Represents a saved lexical state, used for rewinding or nested lexing.
///
/// A `LexerState` captures the current position within a source buffer and any
/// associated leading trivia. It enables resuming lexing from a specific point,
/// supporting speculative parsing and embedded lexers (e.g., for macros or
/// interpolated strings).
class LexerState final {
public:
  LexerState() {}

  /// \returns true if this state has a valid source location.
  bool IsValid() const { return loc.isValid(); }

  /// \brief Advances the current state by the given byte offset.
  ///
  /// \note The returned `LexerState` is a shallow copy with its location
  /// advanced by the specified number of bytes.
  LexerState Advance(unsigned offset) const {
    assert(IsValid());
    return LexerState(loc.getAdvancedLoc(offset));
  }

private:
  explicit LexerState(SrcLoc loc) : loc(loc) {}

  SrcLoc loc;                     ///< The current byte-level location in the source.
  llvm::StringRef leadingTrivia; ///< Optional trivia preceding a token (e.g., whitespace/comments).

  friend class Lexer; ///< Grants `Lexer` access to construct and manipulate `LexerState`.
};

/// \brief Specifies whether the lexer should accept a hashbang line (`#!`) at the start.
enum class HashbangMode : bool {
  Disallowed, ///< Reject `#!` at the beginning of the file (default).
  Allowed     ///< Allow and skip over the hashbang line.
};

/// \brief Controls how the lexer treats comments in the source buffer.
enum class CommentRetentionMode {
  None,               ///< Discard comments entirely.
  AttachToNextToken,  ///< Store comment as metadata on the next token.
  ReturnAsTokens      ///< Emit comments as separate tokens in the stream.
};

/// \brief Describes the style of conflict markers to detect in source.
enum class ConflictMarkerKind {
  Normal,   ///< Standard Git or diff3-style conflict markers.
  Perforce  ///< Perforce-style conflict markers.
};

using TokenCallback = llvm::function_ref<void(const Token &)>;

/// \brief The core class responsible for tokenizing `.stone` source files.
///
/// `Lexer` scans characters from a `SrcUnit`, emitting tokens and collecting
/// diagnostics. It supports nested lexing via `LexerState`, optional comment
/// and trivia tracking, configurable conflict marker behavior, and observer-based
/// side-effect hooks.
class Lexer final {
  SrcUnit unit;
  SrcMgr &sm;

  Token nextToken;
  LexerState state;

  llvm::SmallVector<DiagID, 3> issues;
  LexerObserver *observer = nullptr;

  /// Pointer to the first character of the buffer, even in a lexer that
  /// scans a subrange of the buffer.
  const char *BufferStart;

  /// Pointer to one past the end character of the buffer, even in a lexer
  /// that scans a subrange of the buffer.  Because the buffer is always
  /// NUL-terminated, this points to the NUL terminator.
  const char *BufferEnd;

  /// Pointer to the artificial EOF that is located before BufferEnd.  Useful
  /// for lexing subranges of a buffer.
  const char *ArtificialEOF = nullptr;

  /// If non-null, points to the '\0' character in the buffer where we should
  /// produce a code completion token.
  const char *CodeCompletionPtr = nullptr;

  /// Points to BufferStart or past the end of UTF-8 BOM sequence if it exists.
  const char *ContentStart;

  /// Pointer to the next not consumed character.
  const char *CurPtr;

  Token nextToken;

   /// The location at which the comment of the next token starts. \c nullptr if
  /// the next token doesn't have a comment.
  const char *CommentStart;

  /// If this is not \c nullptr, all tokens after this point are treated as eof.
  /// Used to cut off lexing early when we detect that the nesting level is too
  /// deep.
  const char *LexerCutOffPoint = nullptr;

  Lexer(const Lexer &) = delete;
  void operator=(const Lexer &) = delete;

public:
  /// Constructs a new lexer for the specified source unit.
  ///
  /// \param unit The `.stone` source unit to tokenize.
  /// \param sm   The source manager that owns the input buffer.
  explicit Lexer(SrcUnit unit, SrcMgr &sm);

  /// Constructs a nested lexer over a subrange defined by two saved states.
  ///
  /// \param parent      The parent lexer for context.
  /// \param beginState  Starting state of the subrange.
  /// \param endState    Ending state (exclusive) of the subrange.
  explicit Lexer(Lexer &parent, LexerState beginState, LexerState endState);

  /// Pull-style tokenization: emit the next token into `result`.
  ///
  /// If the lexer has reached the end of the stream, `result` will contain an EOF token.
  void Lex(Token &result);

  /// Push-style tokenization: emit all tokens to the provided callback.
  ///
  /// This includes the final EOF token at the end.
  void Lex(TokenCallback callback);

  /// Set an optional observer to track internal lexing events (e.g., trivia, diagnostics).
  void SetObserver(LexerObserver *observer);

  /// \returns the current observer, if any.
  LexerObserver *GetObserver() const;

  /// \returns the current lexer state (including source location and trivia).
  LexerState GetState() const { return state; }

  /// \returns a read-only view of diagnostic IDs encountered during lexing.
  llvm::ArrayRef<DiagID> GetIssues() const { return issues; }


public:
   /// Returns true if this lexer will produce a code completion token.
  bool IsCodeCompletion() const { return CodeCompletionPtr != nullptr; }


public:
  /// Whether to allow and skip over a hashbang line (`#!`) at the top of the source file.
  HashbangMode HashbangAllowed = HashbangMode::Disallowed;

  /// Type of conflict markers to recognize and report.
  ConflictMarkerKind conflictMarkerKind = ConflictMarkerKind::Normal;

  /// Strategy for retaining or discarding comments/trivia.
  CommentRetentionMode commentRetentionMode = CommentRetentionMode::None;

private:
  /// Internal scanning routine: advances to the next token.
  void Lex();

  /// Records a diagnostic encountered during lexing.
  void Track(DiagID ID) { issues.push_back(ID); }
};

} // namespace stone

#endif // STONE_LEX_LEXER_H
