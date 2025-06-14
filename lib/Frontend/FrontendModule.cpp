#include "stone/Frontend/FrontendModule.h"
#include "stone/AST/DeclState.h"
#include "stone/Frontend/Frontend.h"

using namespace stone;

FrontendModule::FrontendModule(Frontend &frontend)
    : NormalModuleDecl(new(frontend.GetASTSession())
                           DeclState(frontend.GetASTSession())),
      frontend(frontend) {
  GetDeclState()->SetKind(DeclKind::NormalModule);
}

Status FrontendModule::Initialize() {}

// SourceFile *FrontendModule::CreateSourceFile(const InputFile inputFile) {
//   return nullptr;
// }