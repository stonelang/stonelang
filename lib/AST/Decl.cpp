#include "stone/AST/Decl.h"
#include "stone/AST/ASTSession.h"
#include "stone/AST/DeclState.h"
#include "stone/AST/ModuleDecl.h"

using namespace stone;

Decl::Decl(DeclKind kind, ASTSession &session) : ASTUnit(session), kind(kind) {}

DeclState::DeclState(ASTSession &session)
    : session(session), declInfluencerList(session),
      typeInfluencerList(session) {}

JoinDecl *ASTSession::CreateJoinDecl() { return nullptr; }

SpaceDecl *ASTSession::CreateSpaceDecl() { return nullptr; }

UsingDecl *ASTSession::CreateUsingDecl() { return nullptr; }

FunDecl *ASTSession::CreateFunDecl() { return new (*this) FunDecl(*this); }

NormalModuleDecl *ASTSession::CreateNormalModuleDecl() {
  return new (*this) NormalModuleDecl(*this);
}

BuiltinModuleDecl *ASTSession::CreateBuiltinModuleDecl() { return nullptr; }

ForeignModuleDecl *ASTSession::CreateForeignModuleDecl() { return nullptr; }
