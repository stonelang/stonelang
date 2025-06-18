#include "stone/AST/Module.h"
#include "stone/AST/Type.h"
#include "stone/AST/TypeState.h"

using namespace stone;

Module::Module(ModuleContext &context) : context(context) {}

ModuleFile *Module::GetFirstFile() const {
  assert(!files.empty() && "Module has no module files!");
  return files.front();
}
void Module::Flush() {}