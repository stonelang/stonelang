#include "stone/AST/Decl.h"
#include "stone/AST/DeclState.h"

using namespace stone;

// TODO: Passing null parent for now

Decl::Decl(DeclState *DS) : Tree(nullptr), DS(DS) {
  assert(DS && "DeclState is required for Decl");
  DS->SetOwner(this);
}

// JoinDecl *ASTSession::CreateJoinDecl() { return nullptr; }

// SpaceDecl *ASTSession::CreateSpaceDecl() { return nullptr; }

// UsingDecl *ASTSession::CreateUsingDecl() { return nullptr; }

// FunDecl *ASTSession::CreateFunDecl() {
//   // return new (*this) FunDecl(*this);
// }
