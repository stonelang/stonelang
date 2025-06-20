#include "stone/AST/Decl.h"
#include "stone/AST/DeclState.h"

using namespace stone;

// TODO: Passing null parent for now

Decl::Decl(DeclState *DS) : Node(nullptr), DS(DS) {
  assert(DS && "DeclState is required for Decl");
  DS->SetOwner(this);
}

DeclState *Decl::GetDeclState() { return DS; }
DeclKind Decl::GetKind() const { return DS->GetKind(); }
SrcLoc Decl::GetKindLoc() const { return DS->GetKindLoc(); }
Identifier Decl::GetName() const { return DS->GetName(); }
SrcLoc Decl::GetNameLoc() const { return DS->GetNameLoc(); }

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
