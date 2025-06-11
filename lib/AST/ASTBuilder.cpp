#include "stone/AST/ASTBuilder.h"
#include "stone/AST/ASTSession.h"
#include "stone/AST/Decl.h"

using namespace stone;

ASTBuilder::ASTBuilder(const ASTSession &session) : session(session) {}

FunDecl *ASTBuilder::CreateFunDecl() { return nullptr; }