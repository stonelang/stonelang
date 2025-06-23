#ifndef STONE_AST_EXPR_KIND_H
#define STONE_AST_EXPR_KIND_H

#include <cstdint>

namespace stone {

enum class ExprKind : uint8_t {
  None = 0,
#define EXPR(ID, Parent) ID,
#define LAST_EXPR(ID) Count = ID,
#define EXPR_RANGE(ID, FirstID, LastID)                                        \
  First##ID##Expr = FirstID, Last##ID##Expr = LastID,
#include "stone/AST/ExprNode.def"

};

} // namespace stone

#endif // STONE_AST_EXPR_KIND_H
