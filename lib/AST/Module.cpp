#include "stone/AST/Module.h"
#include "stone/AST/Type.h"
#include "stone/AST/TypeState.h"

using namespace stone;

Module::Module(ASTMemory &memory) : ASTUnit(*this), memory(memory) {

#define BUILTIN_TYPE(ID, Parent)                                               \
  Builtin##ID##Type = new (*this) ID##Type(new (*this) BuiltinTypeState(*this));
#include "stone/AST/TypeNode.def"

  // Initialize all of the known identifiers.
#define BUILTIN_IDENTIFIER_WITH_NAME(Name, IdStr)                              \
  Builtin##Name##Identifier = GetIdentifier(IdStr);
#include "stone/AST/BuiltinIdentifiers.def"
}

ModuleFile *Module::GetFirstFile() const {
  assert(!files.empty() && "Module has no module files!");
  return files.front();
}
void Module::Flush() {}