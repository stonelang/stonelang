#include "stone/AST/StmtBuilder.h"
#include "stone/AST/ASTSession.h"

using namespace stone;

StmtBuilder::StmtBuilder(const ASTSession &session) : session(session) {}