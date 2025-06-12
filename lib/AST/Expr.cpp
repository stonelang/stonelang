#include "stone/AST/Expr.h"
#include "stone/AST/ASTSession.h"

using namespace stone;

Expr::Expr(ExprKind kind, ASTSession &session) : ASTUnit(session), kind(kind) {}
