#include "stone/AST/Stmt.h"

using namespace stone;

Stmt::Stmt(StmtKind kind, ASTSession &session) : ASTUnit(session), kind(kind) {}
