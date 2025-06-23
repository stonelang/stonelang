#include "stone/AST/Module.h"
#include "stone/AST/File.h"
#include "stone/AST/ModuleContext.h"
#include "stone/AST/Type.h"
#include "stone/AST/TypeFlight.h"

using namespace stone;

Module::Module(ModuleContext &context) : context(context) {}

File *Module::GetFirstFile() const {
  assert(!files.empty() && "Module has no module files!");
  return files.front();
}
// void Module::Flush() {}