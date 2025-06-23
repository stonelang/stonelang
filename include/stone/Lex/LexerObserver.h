#ifndef STONE_LEX_LEXER_OBSERVER_H
#define STONE_LEX_LEXER_OBSERVER_H

#include "stone/Core/SrcLoc.h"
#include "stone/Diag/DiagID.h"
#include "llvm/ADT/StringRef.h"

namespace stone {

class Token;
/// \brief Interface for observing internal lexer events.
///
/// Observers can be attached to the lexer to intercept trivia, token
/// boundaries, errors, or special conditions such as conflict markers.
class LexerObserver {
public:
  virtual ~LexerObserver() = default;

  /// Called after a token is formed but before it's returned.
  virtual void OnToken(const Token &result) {}

  /// Called when trivia (e.g., comment or whitespace) is encountered.
  virtual void OnTrivia(llvm::StringRef triviaText, SrcLoc loc) {}

  /// Called when a conflict marker is detected in the source.
  virtual void OnConflictMarker(SrcLoc loc) {}

  /// Called when a diagnostic is generated.
  virtual void OnDiagnostic(DiagID ID, SrcLoc loc) {}
};

} // namespace stone

#endif // STONE_LEX_LEXER_OBSERVER_H
