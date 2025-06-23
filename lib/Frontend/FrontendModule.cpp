#include "stone/Frontend/FrontendModule.h"
#include "stone/AST/DeclFlight.h"
#include "stone/Frontend/Frontend.h"

using namespace stone;

FrontendModule::FrontendModule(const Frontend &frontend) : frontend(frontend) {
  underlyingModule = new (frontend.GetMemory()) Module(frontend.GetMemory());
}

// FrontendSpace *Frontend::CreateSpace() {

//   assert(HasASTSession() &&
//          "Cannote create a FrontendModule without an ASTSession!");
//   return new (GetASTSession()) FrontendSpace(*this);
// }

// FrontendSpace::FrontendSpace(Frontend &frontend)
//     : NormalSpaceDecl(new(frontend.GetASTSession())
//                           DeclFlight(frontend.GetASTSession())),
//       frontend(frontend) {
//   GetDeclFlight()->SetKind(DeclKind::NormalModule);
// }

// Status FrontendSpace::Initialize() {}

// // SourceFile *FrontendModule::CreateSourceFile(const Src inputFile) {
// //   return nullptr;
// // }

// Status FrntendSpace::Evaluate() {}