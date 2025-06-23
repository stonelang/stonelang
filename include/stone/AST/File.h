#ifndef STONE_AST_MODULEFILE_H
#define STONE_AST_MODULEFILE_H

#include "stone/AST/Decl.h"
#include "stone/AST/Diagnostics.h"
#include "stone/AST/Node.h"
#include "stone/AST/Scope.h"
#include "stone/Core/SrcUnit.h"

#include "llvm/ADT/SmallVector.h"

namespace stone {

class Module;
/// \brief Stages of semantic and code generation processing for a File.
enum class FileStage : uint8_t {
  None = 1 << 0,
  CompletedParsing = 1 << 1,     ///< Syntax analysis completed
  CompletedScaffolding = 1 << 2, ///< Initial semantic setup completed
  CompletedLogic = 1 << 3,       ///< Full semantic analysis completed
  CompletedCodeGen = 1 << 4      ///< IR generation completed
};

/// \brief Utility to check if a stage flag is set.
inline bool HasStage(FileStage current, FileStage check) {
  return static_cast<uint8_t>(current) & static_cast<uint8_t>(check);
}

/// \brief Represents a single source file in a module.
///
/// Each `File` is tied to a specific buffer, owns a list of top-level
/// declarations, and tracks the semantic/codegen analysis stages.
class File final : public Node {

  SrcUnit unit;
  Module &parent;
  Scope *scope = nullptr; ///< Top-level lexical scope (may be null early on)
  std::vector<Decl *> topLevelDecls; ///< Top-level declarations
  FileStage stage = FileStage::None; ///< Processing stage flags

public:

  /// \brief Constructs a File with a buffer and parent module.
  File(SrcUnit unit, Module &parent);

  /// \returns true if the file has a valid SpaceDecl as its first top-level
  /// declaration.
  explicit operator bool() const {
    return HasFirstDecl() && llvm::cast<SpaceDecl>(GetFirstDecl());
  }

  /// \returns The SrcUnit that initiated this File
  SrcUnit GetSrcUnit() const { return unit; }

  /// \returns the parent of this file which is the module
  Module &GetParent() { return parent; }

  /// \returns The associated top-level lexical scope.
  Scope *GetScope() const { return scope; }

  /// \brief Assigns a scope to this File.
  void SetScope(Scope *S) { scope = S; }

  /// \returns The first top-level declaration in this file, if any.
  Decl *GetFirstDecl() const;

  /// \returns Whether the file contains any top-level declarations.
  bool HasFirstDecl() const;

  /// \brief Appends a new top-level declaration.
  void AddTopLevelDecl(Decl *D) { topLevelDecls.push_back(D); }

  /// \returns All top-level declarations in the file.
  llvm::ArrayRef<Decl *> GetTopLevelDecls() const;

  /// \brief Dumps a human-readable representation of the module file to output.
  void Dump(llvm::raw_ostream &os) const;

public:
  /// \brief Runtime check: is the given artifact a File?
  static bool classof(const Node *node) {
    return node->GetKind() == NodeKind::File;
  }
};

} // namespace stone

#endif // STONE_AST_MODULEFILE_H
