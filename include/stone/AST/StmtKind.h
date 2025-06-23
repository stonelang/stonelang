#ifndef STONE_AST_STMT_KIND_H
#define STONE_AST_STMT_KIND_H

#include <cstdint>

namespace stone {

// DeclKind enum class definition
enum class StmtKind : uint8_t {
  None = 0,
// Core declaration macros
#define STMT(ID, Parent) ID,
// Ranges
#define LAST_STMT(ID) Count = ID,
#define STMT_RANGE(ID, FirstId, LastId) First##ID = FirstId, Last##ID = LastId,
#include "stone/AST/StmtNode.def"
};

} // namespace stone

#endif // STONE_AST_STMT_KIND_H
