#include "stone/AST/DeclFlight.h"
#include "stone/AST/Decl.h"
#include "stone/AST/Memory.h"

using namespace stone;

DeclFlight::DeclFlight(Memory &mem, DeclFlight *parent)
    : mem(mem), parent(parent) {}

// // void Decl::Evaluate(DeclActionKind kind) {
// //   GetDeclFlight()->GetASTSession().GetDeclEvaluator().Evaluate(this,
// kind);
// // }

// DeclFlight::DeclFlight(ASTSession &session)
//     : session(session), declInfluencerList(session),
//       typeInfluencerList(session) {}

// // GenericDeclFlight::GenericDeclFlight(ASTSession &session) :
// DeclFlight(session)
// // {}

// DeclFlight *ASTSession::CreateDeclFlight() {
//   return new (*this) DeclFlight(*this);
// }
