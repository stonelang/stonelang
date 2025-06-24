#ifndef STONE_AST_NODE_H
#define STONE_AST_NODE_H

#include "stone/AST/Allocation.h"
#include "stone/AST/NodeKind.h"

#include "llvm/ADT/PointerUnion.h"
#include "llvm/ADT/SmallVector.h"

namespace stone {

// void Walk(ArtifactWalker &walker);
// void Walk(ArtifactWalker &&walker) { Walk(walker); }
// void Visit(ASTVisitor& visitor);
/// NodeConext { Node* scope ... }
class Module;
class File;
class Decl;
class Type;
class Expr;
class Stmt;

/// \brief The base class for the nodes
class Node : public Allocation<Node> {
protected:
  /// \brief The parent node (generic union type)
  NodeUnion parent;

  /// \brief The list of children (generic union types)
  llvm::SmallVector<NodeUnion, 16> children;

public:
  explicit Node(NodeUnion parent);

  /// \returns The kind of node this instance represents
  NodeKind GetKind() const;

  /// \returns The parent node (as NodeUnion)
  NodeUnion GetParent() const { return parent; }

  /// \brief Sets the parent node
  void SetParent(NodeUnion P) { parent = P; }

  /// \returns a read-only view of the child nodes
  llvm::ArrayRef<NodeUnion> GetChildren() const { return children; }

  /// \brief Add a child to this node
  void AddChild(NodeUnion child) {
    assert(!child.isNull() && "Cannot add null child");
    children.push_back(child);
  }

  /// \brief Type-safe cast of the parent
  template <typename T> T *GetParentAs() const {
    return parent.dyn_cast<T *>();
  }

  /// \brief Type check on the parent
  template <typename T> bool IsParentType() const { return parent.is<T *>(); }
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
  // virtual bool WalkFile(File *MF) { return true; }

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
  // case ArtifactKind::File:
  //   return VisitFile(static_cast<File *>(A));
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
  // virtual void VisitFile(File *MF) {}
};
} // namespace stone

#endif
