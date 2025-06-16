#include "stone/AST/Decl.h"
#include "stone/AST/ASTSession.h"
#include "stone/AST/DeclState.h"
#include "stone/AST/SpaceDecl.h"

using namespace stone;

namespace stone {
static ASTSession &GetASTSession(DeclState *DS) {
  assert(DS && "Every Decl requires a DeclState!");
  return DS->GetASTSession();
}
} // namespace stone
Decl::Decl(DeclState *DS) : ASTUnit(stone::GetASTSession(DS)) {}

DeclState *Decl::GetDeclState() { return DS; }

// void Decl::Evaluate(DeclActionKind kind) {
//   GetDeclState()->GetASTSession().GetDeclEvaluator().Evaluate(this, kind);
// }

DeclState::DeclState(ASTSession &session)
    : session(session), declInfluencerList(session),
      typeInfluencerList(session) {}

// GenericDeclState::GenericDeclState(ASTSession &session) : DeclState(session)
// {}

DeclState *ASTSession::CreateDeclState() {
  return new (*this) DeclState(*this);
}

JoinDecl *ASTSession::CreateJoinDecl() { return nullptr; }

SpaceDecl *ASTSession::CreateSpaceDecl() { return nullptr; }

UsingDecl *ASTSession::CreateUsingDecl() { return nullptr; }

FunDecl *ASTSession::CreateFunDecl() {
  // return new (*this) FunDecl(*this);
}
