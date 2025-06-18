#include "stone/AST/Stmt.h"

using namespace stone;

Stmt::Stmt(StmtKind kind) : Artifact(nullptr), kind(kind) {}
