#include "stone/AST/Stmt.h"
#include "stone/AST/Node.h"
#include "stone/AST/UnderlyingNode.h"

using namespace stone;

Stmt::Stmt(StmtFlight *flight) : Base(nullptr), flight(flight) {}