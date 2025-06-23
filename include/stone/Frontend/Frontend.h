#ifndef STONE_FRONTEND_FRONTEND_H
#define STONE_FRONTEND_FRONTEND_H

#include "stone/AST/ASTSession.h"
#include "stone/AST/LogicOptions.h"
#include "stone/Frontend/FrontendMemoryBuffers.h"
#include "stone/Frontend/FrontendObserver.h"
#include "stone/Frontend/FrontendOptions.h"
#include "stone/Core/CodeGenOptions.h"
#include "stone/Core/DiagnosticOptions.h"
#include "stone/Core/LangOptions.h"
#include "stone/Core/Status.h"

#include "clang/Basic/FileManager.h"
#include "clang/Basic/FileSystemOptions.h"

#include "llvm/ADT/ArrayRef.h"
#include "llvm/ADT/SmallVector.h"
#include "llvm/Option/ArgList.h"
#include <memory>

namespace stone {

class Frontend final {

  FrontendOptions frontendOpts;
  CodeGenOptions codeGenOpts;
  LogicOptions typeCheckerOpts;
  DiagnosticOptions diagOpts;
  clang::FileSystemOptions clangFileSystemOpts;
  std::unique_ptr<llvm::opt::InputArgList> inputArgList;
  FrontendObserver *observer;

  std::unique_ptr<Memory> astMemory;
  std::unique_ptr<ClangImporter> clangImporter;

public:
  Frontend(llvm::StringRef executablePath, llvm::StringRef executableNam);
  Status ParseArgStrings(llvm::ArrayRef<const char *> args);
  FrontendAction GetAction() { return frontendOpts.CurrentAction; }

public:
  FrontendOptions &GetFronendOptions() { return frontendOpts; }
  CodeGenOptions &GetCodeGenOptions() { return codeGenOpts; }
  DiagnosticOptions &GetDiagnosticOptions() { return diagOpts; }
  clang::FileSystemOptions &GetClangFileSystemOptions() {
    return clangFileSystemOpts;
  }
  bool HasObserver() { return observer != nullptr; }
  void SetObserver(FrontendObserver *obs) { observer = obs; }
  FrontendObserver *GetObserver() { return observer; }

private:
  Status SetupClang(llvm::ArrayRef<const char *> args, const char *arg0);

private:
  Status ConvertArgList(llvm::opt::InputArgList &argList);
  Status ParseFrontendOptions();
  Status ParseFrontendAction();
  Status ParseDiagnosticOptions();
  Status ParseTargetOptions();
  Status ParseCodeGenOptions();
};

} // namespace stone

#endif // STONE_FRONTEND_FRONTENDINPUTFILE_H
