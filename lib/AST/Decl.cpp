#include "stone/AST/Decl.h"
#include "stone/AST/ASTSession.h"
#include "stone/AST/DeclState.h"

using namespace stone;

Decl::Decl(DeclKind kind, ASTSession &session) : ASTUnit(session), kind(kind) {}

DeclState::DeclState(ASTSession &session) : session(session) {}