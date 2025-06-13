#include "stone/Compile/Compiling.h"
#include "stone/Compile/Frontend.h"

using namespace stone;

Compiling::Compiling(Frontend &frontend)
    : frontend(frontend), session(new ASTSession()) {
  normalModule = GetASTSession().GetDeclBuilder().BuildNormalModuleDecl();
}

SourceFile *Compiling::CreateSourceFile(const InputFile inputFile) {
  return nullptr;
}