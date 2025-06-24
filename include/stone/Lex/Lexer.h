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

#include "stone/Core/SrcMgr.h"
#include "stone/Core/SrcUnit.h"
#include "stone/Diag/DiagID.h"
#include "stone/Lex/LexerBookmark.h"

#include "llvm/ADT/FunctionExtras.h"
#include "llvm/ADT/SmallVector.h"
#include "llvm/ADT/StringRef.h"

namespace stone {

class LexerObserver;
using TokenCallback = llvm::function_ref<void(const Token &)>;
/// \brief The core class responsible for tokenizing `.stone` source files.
///
/// `Lexer` scans characters from a `SrcUnit`, emitting tokens and collecting
/// diagnostics. It supports nested lexing via `LexerBookmark`, optional comment
/// and trivia tracking, configurable conflict marker behavior, and
/// observer-based side-effect hooks.
class Lexer final {

  SrcUnit &unit;
  SrcMgr &sm;
  Token nextToken;
  LexerBookmark bookmark;
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
  explicit Lexer(SrcUnit &unit, SrcMgr &sm);

  /// Constructs a nested lexer over a subrange defined by two saved bookmarks.
  ///
  /// \param parent      The parent lexer for context.
  /// \param begin  Starting bookmark of the subrange.
  /// \param end    Ending bookmark (exclusive) of the subrange.
  explicit Lexer(Lexer &parent, LexerBookmark begin, LexerBookmark end);

  /// Pull-style tokenization: emit the next token into `result`.
  ///
  /// If the lexer has reached the end of the stream, `result` will contain an
  /// EOF token.
  void Lex(Token &result);

  /// Push-style tokenization: emit all tokens to the provided callback.
  ///
  /// This includes the final EOF token at the end.
  void Lex(TokenCallback callback);

  /// Set an optional observer to track internal lexing events (e.g., trivia,
  /// diagnostics).
  void SetObserver(LexerObserver *observer);

  /// \returns the current observer, if any.
  LexerObserver *GetObserver() const;

  /// \returns the current lexer bookmark (including source location and
  /// trivia).
  LexerBookmark GetBookmark() const { return bookmark; }

public:
  /// Returns true if this lexer will produce a code completion token.
  bool IsCodeCompletion() const { return CodeCompletionPtr != nullptr; }

private:
  /// Internal scanning routine: advances to the next token.
  void Lex();

  /// Records a diagnostic encountered during lexing.
  void Track(DiagID ID) { unit.AddDiag(ID); }

public:
  static void LexUnit(SrcUnit &unit, SrcMgr &sm);
};

} // namespace stone

#endif // STONE_LEX_LEXER_H
