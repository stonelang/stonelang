#ifndef STONE_AST_STMT_H
#define STONE_AST_STMT_H

#include "stone/AST/AST.h"

namespace stone {

enum class StmtKind : uint8_t {
#define STMT(ID, PARENT) ID,
#define LAST_STMT(ID) Last_Stmt = ID,
#define STMT_RANGE(Id, FirstId, LastId)                                        \
  First_##Id##Stmt = FirstId, Last_##Id##Stmt = LastId,
#include "stone/AST/StmtNode.def"
};

class Stmt : public ASTAllocation<Stmt> {
  StmtKind kind;

public:
  Stmt() = delete;
  Stmt(const Stmt &) = delete;
  Stmt(Stmt &&) = delete;
  Stmt &operator=(const Stmt &) = delete;
  Stmt &operator=(Stmt &&) = delete;

public:
  Stmt(StmtKind kind) : kind(kind) {}

public:
  StmtKind GetKind() const { return kind; }
};

} // namespace stone

#endif