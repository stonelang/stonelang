#include "stone/Compile/Compiling.h"
#include "stone/Compile/Frontend.h"

using namespace stone;

Compiling::Compiling(std::unique_ptr<Frontend> frontend)
    : frontend(std::move(frontend)), session(new ASTSession()) {
  normalModule = GetASTSession().GetDeclBuilder().BuildNormalModuleDecl();
}

SourceFile *Compiling::CreateSourceFile(const InputFile inputFile) {
  return nullptr;
}