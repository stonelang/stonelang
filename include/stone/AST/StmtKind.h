#ifndef STONE_AST_STMT_KIND_H
#define STONE_AST_STMT_KIND_H

#include <cstdint>

namespace stone {

enum class StmtKind : uint8_t {
  None = 0,
#define EXPR(ID, Parent) ID,
#define LAST_EXPR(ID) Count = ID,
#define EXPR_RANGE(ID, FirstID, LastID)                                        \
  First##ID##Stmt = FirstID, Last##ID##Stmt = LastID,
#include "stone/AST/StmtNode.def"

};

} // namespace stone

#endif // STONE_AST_STMT_KIND_H
