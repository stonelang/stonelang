#include "stone/AST/Expr.h"
#include "stone/AST/ExprFlight.h"

using namespace stone;

Expr::Expr(ExprFlight *flight) : flight(flight) {}