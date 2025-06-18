#ifndef STONE_AST_EXPR_KIND_H
#define STONE_AST_EXPR_KIND_H

#include <cstdint>

namespace stone {

enum class ExprKind : uint8_t {
  // Core expressions
  New,
  This,
  Call,
  Paren,

  // Literal expressions
  NullLiteral,
  BooleanLiteral,
  StringLiteral,
  IntegerLiteral,
  FloatLiteral,

  // Closure expressions
  Closure,

  // Special expressions
  IfSemi,
  CodeCompletion,
  Type, // Note: represents a type-level expression

  // Always last
  Count
};

// === Range Markers ===

// LiteralExpr range: Null → Float
constexpr ExprKind FirstLiteral = ExprKind::NullLiteral;
constexpr ExprKind LastLiteral = ExprKind::FloatLiteral;

} // namespace stone

#endif // STONE_AST_EXPR_KIND_H
