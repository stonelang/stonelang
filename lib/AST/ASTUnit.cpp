#include "stone/AST/ASTUnit.h"
#include "stone/AST/ASTSession.h"

using namespace stone;

ASTUnit::ASTUnit(ASTSession &session, ASTUnit *parent)
    : session(session), parent(parent) {}

ASTSession &ASTUnit::GetASTSession() { return session; }