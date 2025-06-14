#include "stone/Frontend/Frontend.h"
#include "stone/Support/Options.h"

using namespace stone;

FrontendArgListConverter::FrontendArgListConverter() {}

Status Frontend::ParseArgStrings(llvm::ArrayRef<const char *> args) {
  // Frontend CL options
  unsigned missingArgIndex;
  unsigned missingArgCount;
  frontendOpts.langOpts.InputArgs = std::make_unique<llvm::opt::InputArgList>(
      frontendOpts.GetLangOptions().GetOptTable().ParseArgs(
          args, missingArgIndex, missingArgCount, opts::FrontendOption));

  return GetArgListConverter().Convert(*this);
}

Status FrontendArgListConverter::Convert(Frontend &frontend) {

  return Status::Success();
}