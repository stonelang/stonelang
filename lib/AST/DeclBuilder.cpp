#include "stone/AST/DeclBuilder.h"
#include "stone/AST/ASTSession.h"
#include "stone/AST/Decl.h"

using namespace stone;

DeclBuilder::DeclBuilder(const ASTSession &session) : session(session) {}

JoinDecl *DeclBuilder::BuildJoinDecl() { return nullptr; }

SpaceDecl *DeclBuilder::BuildSpaceDecl() { return nullptr; }

UsingDecl *DeclBuilder::BuildUsingDecl() { return nullptr; }

FunDecl *DeclBuilder::BuildFunDecl() { return nullptr; }

NormalModuleDecl *DeclBuilder::BuildNormalModuleDecl() { return nullptr; }

BuiltinModuleDecl *DeclBuilder::BuildBuiltinModuleDecl() { return nullptr; }

ForeignModuleDecl *DeclBuilder::BuildForeignModuleDecl() { return nullptr; }
