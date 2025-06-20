#include "stone/AST/DeclState.h"
#include "stone/AST/Decl.h"
#include "stone/AST/MemoryManager.h"

using namespace stone;

DeclState::DeclState(MemoryManager &mem, DeclState *parent)
    : mem(mem), parent(parent) {}

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
