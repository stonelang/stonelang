#ifndef STONE_CORE_SRCUNIT_H
#define STONE_CORE_SRCUNIT_H

#include "stone/Core/FileType.h"
#include "stone/Core/Token.h"

#include "llvm/ADT/SmallVector.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/Support/Path.h"

namespace stone {

/// \brief Specifies whether the lexer should accept a hashbang line (`#!`) at
/// the start.
enum class HashbangMode : bool {
  Disallowed, ///< Reject `#!` at the beginning of the file (default).
  Allowed     ///< Allow and skip over the hashbang line.
};

/// \brief Controls how the lexer treats comments in the source buffer.
enum class CommentRetentionMode : uint8_t {
  None = 0,          ///< Discard comments entirely.
  AttachToNextToken, ///< Store comment as metadata on the next token.
  ReturnAsTokens     ///< Emit comments as separate tokens in the stream.
};

/// \brief Describes the style of conflict markers to detect in source.
enum class ConflictMarkerKind : uint8_t {
  Normal,  ///< Standard Git or diff3-style conflict markers.
  Perforce ///< Perforce-style conflict markers.
};

/// \brief Controls how line directives (e.g., `#line`) are interpreted.
enum class LineDirectiveMode : uint8_t {
  Ignore, ///< Treat line directives as comments.
  Parse,  ///< Parse and apply them to remap line/filename.
  Error   ///< Treat line directives as errors.
};

/// \brief Describes leading trivia attached to tokens or tracked in the unit.
enum class TriviaKind uint8_t : uint8_t {
  None, ///< Skip trivia
  Space,
  Tab,
  Newline,
  LineComment,  ///< `// comment`
  BlockComment, ///< `/* comment */`
  DocComment    ///< `/// or /** */`
};

/// \brief Represents a single trivia element (e.g., comment, space).
struct Trivia final {
  TriviaKind kind;
  llvm::StringRef text;
};

/// \brief Represents a `.stone` source file unit tracked by the compiler.
/// This is the central abstraction for lexical control over a source buffer.
class SrcUnit final {

  ///< Source manager-assigned buffer ID.
  unsigned bufferID = 0;

  ///< Normalized input name (e.g., "main.stone" or "-").
  llvm::StringRef name;

  ///< Collected lexical diagnostics.
  // llvm::SmallVector<DiagID, 8> issues;

  ///< Final token stream for this unit.
  llvm::SmallVector<Token, 32> tokens;

  ///< Detached trivia stream (if enabled).
  llvm::SmallVector<Trivia, 32> trivia;

public:
  ///< Trivia settings -- default is None
  TriviaKind triviaKind = TriviaKind::None;

  ///< HashbangMode settings -- default is Disallowed
  HashbangMode hashbangAllowed = HashbangMode::Disallowed;

  ///< ConflictMarkerKind settings -- default is Normal
  ConflictMarkerKind conflictMarkerKind = ConflictMarkerKind::Normal;

  ///< CommentRetentionMode settings -- default is None
  CommentRetentionMode commentRetentionMode = CommentRetentionMode::None;

  ///< LineDirectiveMode settings -- default is Ignore
  LineDirectiveMode lineDirectiveMode = LineDirectiveMode::Ignore;

public:
  /// \brief Constructs a `SrcUnit`. If `bufferID == 0`, the unit is invalid.
  /// \param inputName The file name or `<stdin>`.
  /// \param bufferID The buffer ID assigned by the source manager.
  explicit SrcUnit(llvm::StringRef inputName, unsigned bufferID = 0)
      : bufferID(bufferID),
        name(inputName.equals("<stdin>") ? "-" : inputName) {
    assert(!name.empty() && "File name must not be empty!");
    assert(IsValid() && "Invalid buffer ID!");
    using llvm::sys::path::extension;
    assert(name == "-" || extension(name) == ".stone" &&
                              "SrcUnit must represent a `.stone` file!");
  }

  /// \returns true if this unit is valid (i.e., has a non-zero buffer ID).
  explicit operator bool() const { return IsValid(); }

  /// \returns true if this unit is valid (i.e., has a non-zero buffer ID).
  bool IsValid() const { return bufferID != 0; }

  /// \returns true if this unit represents standard input.
  bool IsStdin() const { return name == "-"; }

  /// \returns the underlying buffer ID assigned by the source manager.
  unsigned GetBufferID() const { return bufferID; }

  /// \returns the normalized file name (non-owning reference).
  const llvm::StringRef &GetName() const { return name; }

  /// \returns a user-facing name for diagnostics ("<stdin>" or real filename).
  llvm::StringRef GetDisplayName() const {
    return IsStdin() ? "<stdin>" : name;
  }

  /// Reset the SrcUnit for another Lex
  void Clear();

  /// Compile-time tag identifying this unit's file kind.
  static constexpr FileType Kind = FileType::Stone;

  /// --- Diagnostic Management ---
  // void AddDiag(DiagID diag) { issues.push_back(diag); }
  // llvm::ArrayRef<DiagID> GetDiags() const { return issues; }
  // void ClearDiags() { issues.clear(); }

  /// --- Token Stream ---
  void AddToken(const Token &tok) { tokens.push_back(tok); }
  llvm::ArrayRef<Token> GetTokens() const { return tokens; }
  void ClearTokens() { tokens.clear(); }

  /// --- Trivia Buffer (optional detached storage) ---
  void AddTrivia(const Trivia &t) { trivia.push_back(t); }
  llvm::ArrayRef<Trivia> GetTrivia() const { return trivia; }
  void ClearTrivia() { trivia.clear(); }

public:
  /// \brief Dumps a textual summary(.stonelex) of this `SrcUnit` to the given
  /// output stream.
  ///
  /// \param out         The stream to write to.
  /// \param dumpTokens  If true, prints each token in order.
  /// \param dumpDiags   If true, prints diagnostic IDs.
  /// \param indent      Optional indentation (useful for nested output).
  void Dump(llvm::raw_ostream &out, bool dumpTokens = true,
            bool dumpDiags = true, llvm::StringRef indent = "") const;
};

} // namespace stone

#endif // STONE_CORE_SRCUNIT_H
