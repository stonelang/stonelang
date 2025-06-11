#ifndef STONE_AST_EXPR_H
#define STONE_AST_EXPR_H

#include "stone/AST/ASTUnit.h"
#include "stone/AST/ASTWalker.h"

namespace stone {

enum class ExprKind : uint8_t {
#define EXPR(Id, Parent) Id,
#define LAST_EXPR(Id) Last_Expr = Id,
#define EXPR_RANGE(Id, FirstId, LastId)                                        \
  First_##Id##Expr = FirstId, Last_##Id##Expr = LastId,
#include "stone/AST/ExprNode.def"

};

class alignas(8) Expr : public ASTUnit {
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

  ASTUnitKind GetUnitKind() override const { return ASTUnitKind::Expr; }

  /// This recursively walks the AST rooted at this expression.
  // Expr *Walk(ASTWalker &walker);
  // Expr *Walk(ASTWalker &&walker) { return Walk(walker); }

public:
  // static bool classof(const Expr *E) {
  //   return D->GetKind() >= ExprKind::FirstValueDecl &&
  //          D->GetKind() <= ExprKind::LastValueDecl;
  // }
  static bool classof(const ASTUnit *unit) {
    return unit->GetUniKind() == ASTKind::Expr;
  }
};


// if: error then "error" 
//                 else if warning then "warn" 
//                 else if debug then "debug" 
//                 else "ok";

class IfSemiExpr : public Expr {};

// auto result = match: value
//     case a then true
//     case b then false
//     else       null;
// match: val case 1 then "one" case 2 then "many"

class MatchExpr : public Expr {};

} // namespace stone

#endif