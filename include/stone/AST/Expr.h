#ifndef STONE_AST_EXPR_H
#define STONE_AST_EXPR_H

#include "stone/AST/Alignments.h"
#include "stone/AST/ExprKind.h"
#include "stone/AST/Node.h"
#include "stone/AST/NodeKind.h"

namespace stone {
class ExprFlight;

/// \brief The base class for all expression nodes in the AST.
///
/// Each expression node is tagged with an `ExprKind` and participates in
/// the node hierarchy using `NodeKind::Expr`. Subclasses are defined via
/// macro expansion using `ExprNode.def`.
class alignas(1 << ExprAlignInBits) Expr : public Node<NodeKind::Expr> {

  ExprFlight *flight = nullptr;

public:
  /// \brief Constructs an expression node of a given kind.
  explicit Expr(ExprFlight *flight) : Node<NodeKind::Expr>(), flight(flight) {}

  /// \brief Returns the kind of this expression.
  ExprKind GetKind() const;

  /// \brief LLVM-style RTTI: Returns true if the node is an expression.
  static bool classof(const Expr *expr) {
    // return expr->GetKind() >= FirstExpr && expr->GetKind() <= LastExpr;
    return true;
  }

  /// \brief LLVM-style RTTI: Returns true if the node is an Expr.
  static bool classof(const NodeBase *node) {
    return node->GetKind() == NodeKind::Expr;
  }
};

/// \brief Defines a concrete Expr subclass corresponding to each ExprKind.
///
/// This macro generates a final class for every expression kind defined in
/// `ExprNode.def`. Each subclass wraps an `ExprKind`, passed to the base
/// `Expr` constructor. A `classof` method is also provided for LLVM-style RTTI.
///
/// Example Expansion:
/// \code
/// class CallExpr final : public Expr {
/// public:
///   explicit CallExpr(ExprKind kind) : Expr(kind) {}
///   static bool classof(const Expr *E);
/// };
/// \endcode
#define EXPR(ID, Parent)                                                       \
  class ID##Expr final : public Parent {                                       \
  public:                                                                      \
    explicit ID##Expr(ExprFlight *flight) : Parent(flight) {}                  \
    static bool classof(const Expr *E);                                        \
  };

#include "stone/AST/ExprNode.def"

} // namespace stone

#endif // STONE_AST_EXPR_H
