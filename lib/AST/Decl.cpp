#include "stone/AST/Decl.h"
#include "stone/AST/DeclState.h"

using namespace stone;

// TODO: Passing null parent for now

Decl::Decl(DeclState *state) : Node(nullptr), state(state) {
  assert(state && "DeclState is required for Decl");
  state->SetOwner(this);
}

DeclState *Decl::GetState() { return state; }
DeclKind Decl::GetKind() const { return state->GetKind(); }
SrcLoc Decl::GetKindLoc() const { return state->GetKindLoc(); }
Identifier Decl::GetName() const { return state->GetName(); }
SrcLoc Decl::GetNameLoc() const { return state->GetNameLoc(); }

bool Decl::classof(const Decl *D) {
  return D->GetKind() >= DeclKind::Join && D->GetKind() < DeclKind::Count;
}

bool Decl::classof(const Artifact *artifact) {
  return artifact->GetArtifactKind() == ArtifactKind::Decl;
}

// JoinDecl *ASTSession::CreateJoinDecl() { return nullptr; }

// SpaceDecl *ASTSession::CreateSpaceDecl() { return nullptr; }

// UsingDecl *ASTSession::CreateUsingDecl() { return nullptr; }

// FunDecl *ASTSession::CreateFunDecl() {
//   // return new (*this) FunDecl(*this);
// }
