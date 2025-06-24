#ifndef STONE_AST_MODULE_H
#define STONE_AST_MODULE_H

#include "stone/AST/Alignments.h"
#include "stone/AST/Identifier.h"
#include "stone/AST/Node.h"

#include "llvm/ADT/SmallVector.h"

namespace stone {

class Scope;
class File;
class ModuleContext;

/// \brief Represents a full compilation unit (e.g., a module).
///
/// A `Module` manages one or more `File` units and maintains their
/// shared context, scope, and dependency graph. It is the top-level container
/// for semantic and code generation phases.
class alignas(1 << ModuleAlignInBits) Module final
    : public Node<NodeKind::Module> {
  ModuleContext
      &context; ///< Owning context for identifier memory, global types, etc.
  Scope *scope =
      nullptr; ///< Optional top-level scope associated with the module.
  llvm::SmallVector<File *, 16> files; ///< Files contained in this module.

  llvm::SmallVector<const Module *>
      deps; ///< List of other modules this module depends on.

public:
  Module(const Module &) = delete;
  void operator=(const Module &) = delete;

public:
  /// \brief Constructs a new module with the given context.
  explicit Module(ModuleContext &context)
      : Node<NodeKind::Module>(), context(context) {}

  /// \returns true if the module contains at least one file.
  explicit operator bool() const { return HasFiles(); }

  /// \returns All files in this module.
  llvm::ArrayRef<File *> GetFiles() const { return files; }

  /// \brief Adds a new file to the module.
  void AddFile(File *file) { files.push_back(file); }

  /// \returns The first file in the module (if any).
  File *GetFirstFile() const;

  /// \returns true if the module has at least one associated file.
  bool HasFiles() const { return !files.empty(); }

  /// \returns The top-level scope associated with the module.
  Scope *GetScope() const { return scope; }

  /// \brief Sets the scope for this module.
  void SetScope(Scope *S) { scope = S; }

  /// \returns All module dependencies (used during import/lookup).
  llvm::ArrayRef<const Module *> GetDeps() const { return deps; }

  /// \brief Adds a new dependency to this module.
  void AddDep(const Module *S) { deps.push_back(S); }

  /// \brief Retrieves or creates an identifier in this module's context.
  Identifier GetIdentifier(llvm::StringRef text) const;

  /// \returns The owning module context.
  ModuleContext &GetContext() { return context; }

  static bool classof(const Node *node) {
    return node->GetKind() == NodeKind::Module;
  }
};

} // namespace stone

#endif
