#include "stone/AST/ModuleDecl.h"
#include "stone/AST/ModuleFile.h"

using namespace stone;

ModuleDecl::ModuleDecl() : DeclContext() {}

ModuleFile *ModuleDecl::GetFirstModuleFile() const { return nullptr; }

bool ModuleDecl::HasFirstModuleFile() const {
  return GetFirstModuleFile() != nullptr;
}
