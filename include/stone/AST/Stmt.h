#ifndef STONE_AST_STMT_H
#define STONE_AST_STMT_H

#include "stone/AST/ASTUnit.h"

namespace stone {

enum class StmtKind : uint8_t {
#define STMT(ID, PARENT) ID,
#define LAST_STMT(ID) Last_Stmt = ID,
#define STMT_RANGE(Id, FirstId, LastId)                                        \
  First_##Id##Stmt = FirstId, Last_##Id##Stmt = LastId,
#include "stone/AST/StmtNode.def"
};

class alignas(8) Stmt : public ASTUnit {
  StmtKind kind;

public:
  Stmt() = delete;
  Stmt(const Stmt &) = delete;
  Stmt(Stmt &&) = delete;
  Stmt &operator=(const Stmt &) = delete;
  Stmt &operator=(Stmt &&) = delete;

public:
  Stmt(StmtKind kind, ASTSession &session);

public:
  StmtKind GetKind() const { return kind; }
  ASTUnitKind GetUnitKind() const override { return ASTUnitKind::Stmt; }
};

} // namespace stone

#endif