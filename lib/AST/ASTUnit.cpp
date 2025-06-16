#include "stone/AST/ASTUnit.h"
#include "stone/AST/ASTSession.h"

using namespace stone;

ASTUnit::ASTUnit(ASTUnit *parent) : parent(parent) {}
