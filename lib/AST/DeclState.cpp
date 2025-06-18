#include "stone/AST/DeclState.h"
#include "stone/AST/Decl.h"

using namespace stone;

// Every DeclState must have a context
DeclState::DeclState(DeclState *parent) : parent(parent) {}

// DeclState *Decl::GetDeclState() { return DS; }

// // void Decl::Evaluate(DeclActionKind kind) {
// //   GetDeclState()->GetASTSession().GetDeclEvaluator().Evaluate(this, kind);
// // }

// DeclState::DeclState(ASTSession &session)
//     : session(session), declInfluencerList(session),
//       typeInfluencerList(session) {}

// // GenericDeclState::GenericDeclState(ASTSession &session) :
// DeclState(session)
// // {}

// DeclState *ASTSession::CreateDeclState() {
//   return new (*this) DeclState(*this);
// }
