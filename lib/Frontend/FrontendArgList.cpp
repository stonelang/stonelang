#include "stone/Frontend/Frontend.h"
#include "stone/Core/Options.h"

using namespace stone;

Status Frontend::ParseArgStrings(llvm::ArrayRef<const char *> args) {
  // Frontend CL options
  unsigned missingArgIndex;
  unsigned missingArgCount;
  frontendOpts.langOpts.InputArgs = std::make_unique<llvm::opt::InputArgList>(
      frontendOpts.GetLangOptions().GetOptTable().ParseArgs(
          args, missingArgIndex, missingArgCount, opts::FrontendOption));

  return Status::Success();
}

Status Frontend::ConvertArgList(llvm::opt::InputArgList &argList) {

  return Status::Success();
}