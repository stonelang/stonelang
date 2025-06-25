#ifndef STONE_CORE_SRCUNIT_H
#define STONE_CORE_SRCUNIT_H

#include "stone/Core/DiagID.h"
#include "stone/Core/FileType.h"
#include "stone/Core/Token.h"

#include "llvm/ADT/SmallVector.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/Support/Path.h"

#include <cstdint>

namespace stone {

//===----------------------------------------------------------------------===//
// Lexer Mode Controls
//===----------------------------------------------------------------------===//

enum class HashbangMode : bool { Disallowed, Allowed };
enum class CommentRetentionMode : uint8_t {
  None = 0,
  AttachToNextToken,
  ReturnAsTokens
};
enum class ConflictMarkerKind : uint8_t { Normal, Perforce };
enum class LineDirectiveMode : uint8_t { Ignore, Parse, Error };

enum class TriviaKind : uint8_t {
  None,
  Space,
  Tab,
  Newline,
  LineComment,
  BlockComment,
  DocComment
};

struct Trivia final {
  TriviaKind kind;
  llvm::StringRef text;
  SrcLoc loc;
  Trivia(TriviaKind kind = TriviaKind::None,
         llvm::StringRef text = llvm::StringRef(), SrcLoc loc = SrcLoc())
      : kind(kind), (text), loc(loc) {}
};

//===----------------------------------------------------------------------===//
// SrcUnit - Represents a source file for lexing and diagnostics
//===----------------------------------------------------------------------===//

class SrcUnit final {
  unsigned bufferID = 0; ///< Assigned by SourceManager.
  llvm::StringRef name;  ///< File name or "-" for stdin.

  llvm::SmallVector<diag::DiagID, 8> issues; ///< Diagnostics encountered.
  llvm::SmallVector<Token, 32> tokens;       ///< All produced tokens.
  llvm::SmallVector<Trivia, 32> trivia;      ///< Detached trivia.

public:
  // Lexing configuration
  TriviaKind triviaKind = TriviaKind::None;
  HashbangMode hashbangMode = HashbangMode::Disallowed;
  ConflictMarkerKind conflictMarkerKind = ConflictMarkerKind::Normal;
  CommentRetentionMode commentRetentionMode = CommentRetentionMode::None;
  LineDirectiveMode lineDirectiveMode = LineDirectiveMode::Ignore;

  explicit SrcUnit(llvm::StringRef inputName, unsigned bufferID = 0)
      : bufferID(bufferID),
        name(inputName.equals("<stdin>") ? "-" : inputName) {
    assert(!name.empty() && "File name must not be empty!");
    assert(IsValid() && "Invalid buffer ID!");
    using llvm::sys::path::extension;
    assert(name == "-" || extension(name) == ".stone" &&
                              "SrcUnit must represent a `.stone` file!");
  }

  // File identity
  bool IsValid() const { return bufferID != 0; }
  bool IsStdin() const { return name == "-"; }
  explicit operator bool() const { return IsValid(); }

  unsigned GetBufferID() const { return bufferID; }
  const llvm::StringRef &GetName() const { return name; }
  llvm::StringRef GetDisplayName() const {
    return IsStdin() ? "<stdin>" : name;
  }

  // Diagnostics
  void AddDiag(diag::DiagID diag) { issues.push_back(diag); }
  llvm::ArrayRef<diag::DiagID> GetDiags() const { return issues; }
  void ClearDiags() { issues.clear(); }

  // Tokens
  void AddToken(const Token &tok) { tokens.push_back(tok); }
  llvm::ArrayRef<Token> GetTokens() const { return tokens; }
  void ClearTokens() { tokens.clear(); }

  // Detached Trivia
  void AddTrivia(const Trivia &t) { trivia.push_back(t); }
  llvm::ArrayRef<Trivia> GetTrivia() const { return trivia; }
  void ClearTrivia() { trivia.clear(); }

  // Reset state for a fresh lex
  void Clear();

  // Dump as `.stonelex`
  void Dump(llvm::raw_ostream &out, bool dumpTokens = true,
            bool dumpDiags = true, llvm::StringRef indent = "") const;
};

} // namespace stone

#endif // STONE_CORE_SRCUNIT_H
