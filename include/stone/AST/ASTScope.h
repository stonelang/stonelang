#ifndef STONE_AST_ASTSCOPE_H
#define STONE_AST_ASTSCOPE_H

#include "stone/AST/ASTAllocation.h"
#include "stone/AST/ASTUnit.h"
#include "stone/AST/Decl.h"

#include "llvm/ADT/ArrayRef.h"
#include "llvm/ADT/PointerIntPair.h"
#include "llvm/ADT/SmallPtrSet.h"
#include "llvm/ADT/SmallVector.h"

namespace stone {

class ASTScope : public ASTAllocation<ASTScope> {
  ASTScope *parent = nullptr;
  ASTUnit *owner = nullptr; // Could be Decl*, Expr*, etc. in future.

  // Declarations introduced in this scope.
  llvm::SmallPtrSet<Decl *, 32> scopeDecls;

  // Children scopes (e.g., for nested functions or blocks).
  llvm::SmallVector<ASTScope *, 4> children;

  // Indicates whether this scope has been lazily expanded.
  bool wasExpanded = false;

public:
  ASTScope(ASTUnit *owner, ASTScope *parent = nullptr)
      : parent(parent), owner(owner) {}

public:
  ASTScope *GetParent() const { return parent; }
  ASTUnit *GetOwner() const { return owner; }

  bool WasExpanded() const { return wasExpanded; }
  void MarkExpanded() { wasExpanded = true; }

  void AddDecl(Decl *D) { scopeDecls.insert(D); }
  void RemoveDecl(Decl *D) { scopeDecls.erase(D); }

  llvm::SmallPtrSetImpl<Decl *> &GetDecls() { return scopeDecls; }
  const llvm::SmallPtrSetImpl<Decl *> &GetDecls() const { return scopeDecls; }

  void AddChild(ASTScope *child) { children.push_back(child); }
  llvm::ArrayRef<ASTScope *> GetChildren() const { return children; }

  /// Debug dump (to be implemented later)
  void Dump() const;

  /// Lookup utility (defer implementation)
  // Decl *Lookup(Identifier id);
};

} // namespace stone

#endif // STONE_AST_ASTSCOPE_H
