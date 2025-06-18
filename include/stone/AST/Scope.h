#ifndef STONE_AST_SCOPE_H
#define STONE_AST_SCOPE_H

#include "stone/AST/Artifact.h"
#include "stone/AST/Decl.h"
#include "stone/AST/MemoryAllocation.h"

#include "llvm/ADT/ArrayRef.h"
#include "llvm/ADT/PointerIntPair.h"
#include "llvm/ADT/SmallPtrSet.h"
#include "llvm/ADT/SmallVector.h"

namespace stone {

/// Handled in scaffolding
class Scope : public MemoryAllocation<Scope> {
  Scope *parent = nullptr;
  Artifact *owner = nullptr; // Could be Decl*, Expr*, etc. in future.

  // Declarations introduced in this scope.
  llvm::SmallPtrSet<Decl *, 32> scopeDecls;

  // Children scopes (e.g., for nested functions or blocks).
  llvm::SmallVector<Scope *, 4> children;

  // Indicates whether this scope has been lazily expanded.
  bool wasExpanded = false;

public:
  Scope(Artifact *owner, Scope *parent = nullptr)
      : parent(parent), owner(owner) {}

public:
  Scope *GetParent() const { return parent; }
  Artifact *GetOwner() const { return owner; }

  bool WasExpanded() const { return wasExpanded; }
  void MarkExpanded() { wasExpanded = true; }

  void AddDecl(Decl *D) { scopeDecls.insert(D); }
  void RemoveDecl(Decl *D) { scopeDecls.erase(D); }

  llvm::SmallPtrSetImpl<Decl *> &GetDecls() { return scopeDecls; }
  const llvm::SmallPtrSetImpl<Decl *> &GetDecls() const { return scopeDecls; }

  void AddChild(Scope *child) { children.push_back(child); }
  llvm::ArrayRef<Scope *> GetChildren() const { return children; }

  /// Debug dump (to be implemented later)
  void Dump() const;

  /// Lookup utility (defer implementation)
  // Decl *Lookup(Identifier id);
};

} // namespace stone

#endif // STONE_AST_ASTSCOPE_H
