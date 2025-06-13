#include "stone/Driver/Run.h"
#include "stone/Driver/Driver.h"
#include "stone/Support/LLVMInit.h"

using namespace stone;

int stone::Run(llvm::ArrayRef<const char *> args, const char *arg0,
               void *mainAddr) {

  FINISH_LLVM_INIT();

  auto mainExecutablePath = llvm::sys::fs::getMainExecutable(arg0, mainAddr);
  auto mainExecutableName = llvm::sys::path::stem(arg0);
  auto driver =
      std::make_unique<Driver>(mainExecutablePath, mainExecutableName);

  if (driver->ParseArgStrings(args).IsError()) {
  }

  return 0;
}
