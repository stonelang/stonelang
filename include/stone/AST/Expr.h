#ifndef STONE_AST_EXPR_H
#define STONE_AST_EXPR_H

#include "stone/AST/AST.h"
#include "stone/AST/ASTWalker.h"

namespace stone {

enum class ExprKind : uint8_t {
#define EXPR(Id, Parent) Id,
#define LAST_EXPR(Id) Last_Expr = Id,
#define EXPR_RANGE(Id, FirstId, LastId)                                        \
  First_##Id##Expr = FirstId, Last_##Id##Expr = LastId,
#include "stone/AST/ExprNode.def"

};

class Expr : public ASTAllocation<Expr> {
  ExprKind kind;

public:
  Expr() = delete;
  Expr(const Expr &) = delete;
  Expr(Expr &&) = delete;
  Expr &operator=(const Expr &) = delete;
  Expr &operator=(Expr &&) = delete;

public:
  Expr(ExprKind kind) : kind(kind) {}

  // ExprKind kind, Type qualTy, ExprValueKind VK, ExprObjectKind OK

public:
  ExprKind GetKind() const { return kind; }
  /// This recursively walks the AST rooted at this expression.
  Expr *Walk(ASTWalker &walker);
  Expr *Walk(ASTWalker &&walker) { return Walk(walker); }
};
