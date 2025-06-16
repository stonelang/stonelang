#include "stone/Frontend/Frontend.h"
#include "stone/Support/Options.h"

using namespace stone;

Frontend::Frontend(llvm::StringRef executablePath,
                   llvm::StringRef executableNam) {
  frontendOpts.langOpts.MainExecutablePath = executablePath;
  frontendOpts.langOpts.MainExecutableName = executableNam;
}

Status Frontend::SetupASTSession() {
  // Simple for now
  if (!HasASTSession()) {
    /// Load the evaluators
    session = std::make_unique<ASTSession>();
  }
  return Status::Success();
}
