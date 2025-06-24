#ifndef STONE_AST_NODE_H
#define STONE_AST_NODE_H

#include "stone/AST/Allocation.h"
#include "stone/AST/NodeKind.h"

#include "llvm/ADT/SmallVector.h"

namespace stone {

class Module;
class File;
class Decl;
class Type;
class Expr;
class Stmt;

/// \brief The base class for all AST nodes.
///
/// Stores classification and tree structure (parent and children).
/// Do not instantiate directly — use `Node<K>` instead.
class NodeBase : public Allocation<NodeBase> {
protected:
  NodeKind kind = NodeKind::None;
  NodeBase *parent = nullptr;
  llvm::SmallVector<NodeBase *, 8> children;

  explicit NodeBase(NodeKind kind, NodeBase *parent = nullptr)
      : kind(kind), parent(parent) {}

public:
  NodeBase(const NodeBase &) = delete;
  NodeBase &operator=(const NodeBase &) = delete;

  NodeKind GetKind() const;
  NodeBase *GetParent() const { return parent; }
  void SetParent(NodeBase *P) { parent = P; }

  llvm::ArrayRef<NodeBase *> GetChildren() const { return children; }

  void AddChild(NodeBase *child) {
    assert(child && "Cannot add null child");
    children.push_back(child);
  }

  template <typename T> T *GetParentAs() const {
    return llvm::dyn_cast_or_null<T>(parent);
  }

  template <typename T> bool IsParentType() const {
    return llvm::isa<T>(parent);
  }
};

/// \brief A strongly typed AST node with static kind information.
template <NodeKind K> class Node : public NodeBase {
public:
  static constexpr NodeKind kind = K;
  explicit Node(NodeBase *parent = nullptr) : NodeBase(K, parent) {}
};

/// \brief AST walker for structured traversal.
class Walker {
public:
  Walker() = default;

  // Hook points:
  // bool Walk(NodeBase *node);
  // bool WalkDecl(Decl *D);
  // bool WalkType(Type *T);
  // bool WalkExpr(Expr *E);
  // bool WalkStmt(Stmt *S);
  // bool WalkModule(Module *M);
  // bool WalkFile(File *F);
};

/// \brief AST visitor for diagnostics or transformation.
class Visitor {
public:
  Visitor() = default;

  // Dispatch entry point:
  // void Visit(NodeBase *node);

  // Optional fine-grained hooks:
  // void VisitDecl(Decl *D);
  // void VisitType(Type *T);
  // void VisitExpr(Expr *E);
  // void VisitStmt(Stmt *S);
  // void VisitModule(Module *M);
  // void VisitFile(File *F);
};

} // namespace stone

#endif // STONE_AST_NODE_H
