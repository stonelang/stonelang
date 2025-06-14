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

public:
  static bool classof(const ASTUnit *unit) {
    return unit->GetUnitKind() == ASTUnitKind::Stmt;
  }
};

class BraceStmt final : public Stmt {
public:
  BraceStmt(ASTSession &session) : Stmt(StmtKind::Brace, session) {}
};

class ResturnStmt final : public Stmt {
public:
  ResturnStmt(ASTSession &session) : Stmt(StmtKind::Return, session) {}
};

class FreeStmt final : public Stmt {
public:
  FreeStmt(ASTSession &session) : Stmt(StmtKind::Free, session) {}
};

} // namespace stone

#endif