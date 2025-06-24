#include "stone/AST/Stmt.h"
#include "stone/AST/Node.h"
#include "stone/AST/StmtFlight.h"

using namespace stone;

Stmt::Stmt(StmtFlight *flight) : Node(this), flight(flight) {}