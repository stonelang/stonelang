#ifndef STONE_AST_TREE_H
#define STONE_AST_TREE_H

#include "stone/AST/Artifact.h"

#include "llvm/ADT/SmallVector.h"

namespace stone {

// void Walk(ArtifactWalker &walker);
// void Walk(ArtifactWalker &&walker) { Walk(walker); }
// void Visit(ASTVisitor& visitor);
/// NodeConext { Node* scope ... }

class Node : public Artifact {
  Node *parent = nullptr;
  llvm::SmallVector<Node *, 16> children;

public:
  Node(Node *parent = nullptr) : parent(parent) {}

public:
  bool HasParent() const { return parent != nullptr; }
  Node *GetParent() { return parent; }
  void SetParent(Node *node) { parent = node; }
  void AddChild(Node *node);

  llvm::ArrayRef<Node *> GetChildren() const { return children; }
  llvm::SmallVectorImpl<Node *> &GetChildrenMutable() { return children; }
  bool HasChildren() const { return !children.empty(); }
  size_t GetNumChildren() const { return children.size(); }

  // Optional: For range-based for-loops
  auto begin() const { return children.begin(); }
  auto end() const { return children.end(); }

public:
  virtual ArtifactKind GetArtifactKind() const = 0;
};

class Walker {
public:
  Walker();

  // virtual ~ArtifactWalker() = default;

  // /// Unified dispatcher
  // virtual bool Walk(Node *A) {

  // }

  // // === Hook points (optional) ===
  // virtual bool WalkDecl(Decl *D) { return true; }
  // virtual bool WalkType(Type *T) { return true; }
  // virtual bool WalkExpr(Expr *E) { return true; }
  // virtual bool WalkStmt(Stmt *S) { return true; }

  // virtual bool WalkModule(Module *M) { return true; }
  // virtual bool WalkModuleFile(ModuleFile *MF) { return true; }

  // virtual void Walk(Node *node) = 0;
};

class Visitor {
public:
  Visitor();

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

  // virtual void Visit(Node *node) {} // fallback

  // virtual void VisitDecl(Decl *decl) {
  //   Visit(static_cast<Node *>(decl));
  // }

  // virtual void VisitType(Type *type) {
  //   Visit(static_cast<Node *>(type));
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
