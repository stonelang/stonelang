#include "stone/AST/Decl.h"
#include "stone/AST/ASTSession.h"
#include "stone/AST/DeclState.h"

using namespace stone;

DeclState::DeclState(ASTContext &astContext) : astContext(astContext) {}