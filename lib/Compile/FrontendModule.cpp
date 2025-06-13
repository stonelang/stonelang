#include "stone/Compile/FrontendModule.h"
#include "stone/Compile/Frontend.h"

using namespace stone;

FrontendModule::FrontendModule(Frontend &frontend)
    : NormalModuleDecl(frontend.GetASTSession()), frontend(frontend) {}

// SourceFile *FrontendModule::CreateSourceFile(const InputFile inputFile) {
//   return nullptr;
// }