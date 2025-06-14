#include "stone/AST/ASTUnit.h"

using namespace stone;

ASTUnit::ASTUnit(ASTSession &session, ASTUnit *parent)
    : session(session), parent(parent) {}