#ifndef STONE_AST_NODE_H
#define STONE_AST_NODE_H

#include "stone/AST/Allocation.h"
#include "stone/AST/NodeKind.h"

#include "llvm/ADT/PointerUnion.h"
#include "llvm/ADT/SmallVector.h"

namespace stone {

class Module;
class File;
class Decl;
class Type;
class Expr;
class Stmt;

/// \brief A low-level root base class for all AST nodes.
///
/// You may derive from this to create other tagged or specialized node roots.
/// For now, this base serves as a clean anchor for allocation or static casting.
class NodeBase : public Allocation<NodeBase> {
  // Empty by design, for tagging and layering.
};

/// \brief The base class for all AST nodes.
class Node : public NodeBase {
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

/// \brief AST traversal driver for structured tree walking.
///
/// Meant to provide optional hooks for tree traversal passes.
/// You can subclass and override only what you need.
class Walker {
public:
  Walker() = default;

  // Hook points – uncomment as needed for structure-driven walks.
  // bool Walk(Node *node);
  // bool WalkDecl(Decl *D);
  // bool WalkType(Type *T);
  // bool WalkExpr(Expr *E);
  // bool WalkStmt(Stmt *S);
  // bool WalkModule(Module *M);
  // bool WalkFile(File *F);
};

/// \brief AST visitor for inspection or transformation.
class Visitor {
public:
  Visitor() = default;

  // Entry points and fine-grained visit hooks – enable on demand.
  // void Visit(Node *node);
  // void VisitDecl(Decl *D);
  // void VisitType(Type *T);
  // void VisitExpr(Expr *E);
  // void VisitStmt(Stmt *S);
  // void VisitModule(Module *M);
  // void VisitFile(File *F);
};

} // namespace stone

#endif // STONE_AST_NODE_H
