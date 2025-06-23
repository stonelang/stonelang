#include "stone/AST/Decl.h"
#include "stone/AST/DeclFlight.h"
#include "stone/AST/Module.h"
#include "stone/AST/Node.h"
#include "stone/AST/UnderlyingNode.h"
using namespace stone;

// TODO: Passing null parent for now

// Decl::Decl(DeclFlight *flight): Note<NodeKind>, flight(flight) {
//   assert(flight && "DeclFlight is required for Decl");
//   flight->SetOwner(this);
// }

Decl::Decl(DeclFlight *parent) : Base(nullptr), flight(flight) {

  assert(flight && "DeclFlight is required for Decl");
  flight->SetOwner(this);
}

DeclFlight *Decl::GetFlight() { return flight; }
DeclKind Decl::GetKind() const { return flight->GetKind(); }
Identifier Decl::GetName() const { return flight->GetName(); }
SrcLoc Decl::GetLoc() const { return flight->GetNameLoc(); }

bool Decl::classof(const Decl *D) {
  return D->GetKind() >= DeclKind::Join && D->GetKind() < DeclKind::Count;
}

bool Decl::classof(const Node *node) {
  return node->GetKind() == NodeKind::Decl;
}

// JoinDecl *ASTSession::CreateJoinDecl() { return nullptr; }

// SpaceDecl *ASTSession::CreateSpaceDecl() { return nullptr; }

// UsingDecl *ASTSession::CreateUsingDecl() { return nullptr; }

// FunDecl *ASTSession::CreateFunDecl() {
//   // return new (*this) FunDecl(*this);
// }
