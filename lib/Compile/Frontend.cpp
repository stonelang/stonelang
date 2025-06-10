#include "stone/Compile/Frontend.h"

using namespace stone;

Frontend::Frontend() {}

Status Frontend::ParseArgStrings(llvm::ArrayRef<const char *> args) {
  return Status::Done();
}
