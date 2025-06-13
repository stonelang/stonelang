#include "stone/Frontend/FrontendModule.h"
#include "stone/Frontend/Frontend.h"

using namespace stone;

FrontendModule::FrontendModule(Frontend &frontend)
    : NormalModuleDecl(frontend.GetASTSession()), frontend(frontend) {}

// SourceFile *FrontendModule::CreateSourceFile(const InputFile inputFile) {
//   return nullptr;
// }