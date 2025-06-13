#include "stone/Driver/Driver.h"

using namespace stone;

Driver::Driver(llvm::StringRef executablePath, llvm::StringRef executableNam) {
  langOpts.MainExecutablePath = executablePath;
  langOpts.MainExecutableName = executableNam;
}

Status Driver::ParseArgStrings(llvm::ArrayRef<const char *> args) {

  unsigned IncludedFlagsBitmask = 0;
  unsigned ExcludedFlagsBitmask = opts::NotForDriverOption;
  unsigned MissingArgIndex, MissingArgCount;
  langOpts.InputArgs = std::make_unique<llvm::opt::InputArgList>(
      langOpts.GetOptTable().ParseArgs(args, MissingArgIndex, MissingArgCount,
                                       IncludedFlagsBitmask,
                                       ExcludedFlagsBitmask));
  return Status::Success();
}
