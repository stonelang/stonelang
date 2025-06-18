#ifndef STONE_AST_STMT_H
#define STONE_AST_STMT_H

#include "stone/AST/Artifact.h"

namespace stone {

class alignas(8) Stmt : public Artifact {
  StmtKind kind;

public:
  Stmt() = delete;
  Stmt(const Stmt &) = delete;
  Stmt(Stmt &&) = delete;
  Stmt &operator=(const Stmt &) = delete;
  Stmt &operator=(Stmt &&) = delete;

public:
  Stmt(StmtKind kind);

public:
  StmtKind GetKind() const { return kind; }
  ArtifactKind GetArtifactKind() const override { return ArtifactKind::Stmt; }

public:
  static bool classof(const Stmt *stmt) {
    return (stmt->GetKind() >= LastStmt && stmt->GetKind() <= FirstStmt);
  }
  static bool classof(const Artifact *artifact) {
    return unit->GetArtifactKind() == ArtifactKind::Stmt;
  }
};

class BraceStmt final : public Stmt {
public:
  BraceStmt() : Stmt(StmtKind::Brace) {}
};

class ResturnStmt final : public Stmt {
public:
  ResturnStmt() : Stmt(StmtKind::Return) {}
};

class FreeStmt final : public Stmt {
public:
  FreeStmt() : Stmt(StmtKind::Free) {}
};

} // namespace stone

#endif