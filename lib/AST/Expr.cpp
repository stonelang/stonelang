#include "stone/AST/Expr.h"

using namespace stone;

Expr::Expr(ExprKind kind) : Artifact(nullptr), kind(kind) {}
