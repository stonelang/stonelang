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

/// \file Node.h
/// \brief Defines the base `Node` structure for representing AST and semantic
/// units
///        such as modules, declarations, expressions, statements, and types.

template <NodeKind Kind, typename Derived>
class Node : public Allocation<Derived> {
protected:
  ///< Enumerated type for fast runtime classification
  NodeKind kind = Kind;
  ///< Pointer to this node's parent in the AST hierarchy
  Node *parent = nullptr;

  /// \brief The list of child nodes in this subtree
  llvm::SmallVector<Node *, 16> children;

public:
  using CurrentNode = Node<Kind, Derived>;

  explicit Node(CurrentNode *parent = nullptr);

public:
  /// \returns The `NodeKind` tag indicating what kind of node this is
  NodeKind GetKind() const { return kind; }

  /// \returns The parent node in the AST, if any
  Node *GetParent() const { return parent; }

  /// \brief Sets the parent of this node
  void SetParent(Node *p) { parent = p; }

  /// \returns A read-only view of the children of this node
  llvm::ArrayRef<Node *> GetChildren() const { return children; }

  /// \brief Adds a non-null child node to this node's children list
  /// \param child The child node to add
  void AddChild(Node *child);

  /// \brief Type check against the underlying pointer
  /// \tparam T The desired type (e.g., `Decl`, `Expr`)
  /// \returns `true` if the stored value is of type `T*`
  // template <typename T> bool is() const { return underlying.is<T *>(); }

  // /// \brief Retrieves the stored pointer if it matches type `T*`
  // /// \tparam T The expected type (e.g., `Decl`, `Expr`)
  // /// \returns Pointer to type `T`, or undefined behavior if type mismatch
  // template <typename T> T *get() const { return underlying.get<T *>(); }
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
