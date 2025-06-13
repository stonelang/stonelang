#include "stone/Compile/Frontend.h"
#include "stone/Support/Options.h"

using namespace stone;

Frontend::Frontend(llvm::StringRef executablePath,
                   llvm::StringRef executableNam) {

  frontendOpts.langOpts.MainExecutablePath = executablePath;
  frontendOpts.langOpts.MainExecutableName = executableNam;
}

Status Frontend::ParseArgStrings(llvm::ArrayRef<const char *> args) {

  // Frontend CL options
  unsigned missingArgIndex;
  unsigned missingArgCount;
  frontendOpts.langOpts.InputArgs = std::make_unique<llvm::opt::InputArgList>(
      frontendOpts.GetLangOptions().GetOptTable().ParseArgs(
          args, missingArgIndex, missingArgCount, opts::FrontendOption));

  return Status::Success();
}
