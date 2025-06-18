#ifndef STONE_AST_TREE_H
#define STONE_AST_TREE_H

#include "stone/AST/Artifact.h"

#include "llvm/ADT/SmallVector.h"

namespace stone {

/// TreeConext { Tree* scope ... }
class Tree : public Artifact {
  Tree *parent = nullptr;
  llvm::SmallVector<Tree *, 16> children;

public:
  Tree(Tree *parent = nullptr) : parent(parent) {}

public:
  bool HasParent() const { return parent != nullptr; }
  Artifact *GetParent() { return parent; }
  void SetParent(Tree *tree) { parent = tree; }

  virtual ArtifactKind GetArtifactKind() const = 0;
};

class TreeWalker {
public:
  TreeWalker();

  // virtual ~ArtifactWalker() = default;

  // /// Unified dispatcher
  // virtual bool Walk(Tree *A) {

  // }

  // // === Hook points (optional) ===
  // virtual bool WalkDecl(Decl *D) { return true; }
  // virtual bool WalkType(Type *T) { return true; }
  // virtual bool WalkExpr(Expr *E) { return true; }
  // virtual bool WalkStmt(Stmt *S) { return true; }

  // virtual bool WalkModule(Module *M) { return true; }
  // virtual bool WalkModuleFile(ModuleFile *MF) { return true; }

  // virtual void Walk(Tree *node) = 0;
};

class TreeVisitor {
public:
  TreeVisitor();

  // virtual ~ArtifactVisitor() = default;

  // virtual void Visit(Artifact *A) {
  // if (!A) return;

  // switch (A->GetArtifactKind()) {
  // case ArtifactKind::Decl:
  //   return VisitDecl(static_cast<Decl *>(A));
  // case ArtifactKind::Type:
  //   return VisitType(static_cast<Type *>(A));
  // case ArtifactKind::Expr:
  //   return VisitExpr(static_cast<Expr *>(A));
  // case ArtifactKind::Stmt:
  //   return VisitStmt(static_cast<Stmt *>(A));
  // case ArtifactKind::Module:
  //   return VisitModule(static_cast<Module *>(A));
  // case ArtifactKind::ModuleFile:
  //   return VisitModuleFile(static_cast<ModuleFile *>(A));
  // default:
  //   return;
  // }

  // virtual void Visit(Tree *node) {} // fallback

  // virtual void VisitDecl(Decl *decl) {
  //   Visit(static_cast<TreeArtifact *>(decl));
  // }

  // virtual void VisitType(Type *type) {
  //   Visit(static_cast<TreeArtifact *>(type));
  // }

  // Optional fine-grained visits
  // virtual void VisitDecl(Decl *D) {}
  // virtual void VisitType(Type *T) {}
  // virtual void VisitExpr(Expr *E) {}
  // virtual void VisitStmt(Stmt *S) {}

  // virtual void VisitModule(Module *M) {}
  // virtual void VisitModuleFile(ModuleFile *MF) {}
};
} // namespace stone

#endif
