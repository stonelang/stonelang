#ifndef STONE_AST_SCOPE_H
#define STONE_AST_SCOPE_H

#include "stone/AST/Artifact.h"
#include "stone/AST/Decl.h"

#include "stone/AST/Identifier.h"
#include "stone/AST/MemoryAllocation.h"

#include "llvm/ADT/ArrayRef.h"
#include "llvm/ADT/PointerIntPair.h"
#include "llvm/ADT/SmallPtrSet.h"
#include "llvm/ADT/SmallVector.h"

namespace stone {
class Scope;

using ScopeList = llvm::SmallVector<Scope *, 16>;

/// Handled in scaffolding
// class Scope : public Artifact {
//   Scope *parent = nullptr;

//   // Declarations introduced in this scope.
//   llvm::SmallPtrSet<Decl *, 32> scopeDecls;

//   // Children scopes (e.g., for nested functions or blocks).
//   llvm::SmallVector<Scope *, 4> children;

//   // Indicates whether this scope has been lazily expanded.
//   bool wasExpanded = false;

// public:
//   Scope(Artifact *owner, Scope *parent = nullptr)
//       : parent(parent), owner(owner) {}

// public:
//   Scope *GetParent() const { return parent; }
//   Artifact *GetOwner() const { return owner; }

//   bool WasExpanded() const { return wasExpanded; }
//   void MarkExpanded() { wasExpanded = true; }

//   void AddDecl(Decl *D) { scopeDecls.insert(D); }
//   void RemoveDecl(Decl *D) { scopeDecls.erase(D); }

//   llvm::SmallPtrSetImpl<Decl *> &GetDecls() { return scopeDecls; }
//   const llvm::SmallPtrSetImpl<Decl *> &GetDecls() const { return scopeDecls;
//   }

//   void AddChild(Scope *child) { children.push_back(child); }
//   llvm::ArrayRef<Scope *> GetChildren() const { return children; }

//   /// Debug dump (to be implemented later)
//   void Dump() const;

//   /// Lookup utility (defer implementation)
//   // Decl *Lookup(Identifier id);
// };

class Scope : public Artifact {
  Scope *parent = nullptr;
  llvm::DenseMap<Identifier, Decl *> symbols;

public:
  Scope(Scope *parent = nullptr) : parent(parent) {}

public:
  Decl *Find(Identifier identifier) const;
  void Add(Decl *member) {
    // symbols[member->GetDeclState()->GetIdentifier()] = member;
  }

  ArtifactKind GetArtifactKind() const override { return ArtifactKind::Scope; }
  Scope *GetParent() const { return parent; }

public:
  static bool classof(const Artifact *artifact) {
    return artifact->GetArtifactKind() == ArtifactKind::Scope;
  }

  /// Debug dump (to be implemented later)
  void Dump() const;

  /// Lookup utility (defer implementation)
  // Decl *Lookup(Identifier id);
};

} // namespace stone

#endif // STONE_AST_ASTSCOPE_H
