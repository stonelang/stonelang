#include "stone/Frontend/FrontendModule.h"
#include "stone/AST/DeclState.h"
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
//                           DeclState(frontend.GetASTSession())),
//       frontend(frontend) {
//   GetDeclState()->SetKind(DeclKind::NormalModule);
// }

// Status FrontendSpace::Initialize() {}

// // SourceFile *FrontendModule::CreateSourceFile(const InputFile inputFile) {
// //   return nullptr;
// // }

// Status FrntendSpace::Evaluate() {}