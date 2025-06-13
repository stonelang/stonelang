#include "stone/AST/DeclBuilder.h"
#include "stone/AST/ASTSession.h"
#include "stone/AST/Decl.h"
#include "stone/AST/ModuleDecl.h"

using namespace stone;

DeclBuilder::DeclBuilder(ASTSession &session) : session(session) {}

JoinDecl *DeclBuilder::BuildJoinDecl() { return nullptr; }

SpaceDecl *DeclBuilder::BuildSpaceDecl() { return nullptr; }

UsingDecl *DeclBuilder::BuildUsingDecl() { return nullptr; }

FunDecl *DeclBuilder::BuildFunDecl() { return nullptr; }

NormalModuleDecl *DeclBuilder::BuildNormalModuleDecl() {
  return new (session) NormalModuleDecl(session);
}

BuiltinModuleDecl *DeclBuilder::BuildBuiltinModuleDecl() { return nullptr; }

ForeignModuleDecl *DeclBuilder::BuildForeignModuleDecl() { return nullptr; }
