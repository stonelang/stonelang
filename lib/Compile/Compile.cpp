#include "stone/Compile/Compile.h"
#include "stone/Frontend/FrontendModule.h"
#include "stone/Support/LLVMInit.h"

using namespace stone;

int stone::Compile(llvm::ArrayRef<const char *> args, const char *arg0,
                   void *mainAddr, FrontendObserver *observer) {

  FINISH_LLVM_INIT();

  auto mainExecutablePath = llvm::sys::fs::getMainExecutable(arg0, mainAddr);
  auto mainExecutableName = llvm::sys::path::stem(arg0);
  Frontend frontend(mainExecutablePath, mainExecutableName);

  if (args.empty()) {
    // frontend.GetDiags().diagnose(SrcLoc(), diag::error_no_input_files);
    // return FinishCompile(Status::Error());
  }
  if (!frontend.ParseArgStrings(args).IsSuccess()) {
    return 1;
  }
  switch (frontend.GetMode()) {
  case FrontendMode::PrintHelp: {
    // instance.GetInvocation().GetCompilerOptions().PrintHelp();
    // return FinishCompile();
  }
  case FrontendMode::PrintHelpHidden: {
    // instance.GetInvocation().GetCompilerOptions().PrintHelp(true);
    // return FinishCompile();
  }
  case FrontendMode::PrintVersion: {
    // stone::PrintCompilerVersion();
    // return FinishCompile();
  }
  case FrontendMode::PrintFeature: {
    // stone::PrintCompilerFeatures();
    // return FinishCompile();
  }
  default: {
    break;
  }
  }

  frontend.SetObserver(observer);
  // Simple for now
  if (!frontend.SetupASTSession().IsSuccess()) {
    return 1;
  }

  FrontendModule *frontendModule = stone::CreateFrontendModule(frontend);
  if (!frontendModule->Initialize().IsSuccess()) {
    return 1;
  }

  if (!stone::PerformCompile(*frontendModule).IsSuccess()) {
    return 1;
  }
  return 0;
}

FrontendModule *stone::CreateFrontendModule(Frontend &frontend) {
  assert(frontend.HasASTSession() &&
         "Cannote create a FrontendModule without an ASTSession!");
  return new (frontend.GetASTSession()) FrontendModule(frontend);
}
/// \return status of compile
Status stone::PerformCompile(FrontendModule &fm) {

  switch (fm.GetFrontend().GetMode()) {
  case FrontendMode::Parse: {
    return stone::PerformParse(
        fm, [&](FrontendModule &fm) { return Status::Success(); });
  }
  }
  Status::Success();
}

/// \return true if we compiled an ir file.
Status stone::PerformCompileLLVM(FrontendModule &fm) { Status::Success(); }

/// \return true if syntax analysis is successful
Status stone::PerformParse(FrontendModule &fm,
                           FrontendModuleCallback callback) {

  // when we parse, we add to the mdou
  // FrontendModule.GetFrontend().GetFrontendOptions().ForEachInput();

  // frontendModule.GetModule().AddSourceFile()
  Status::Success();
}

// \return true if syntax analysis is successful
Status stone::PerformParse(SourceFile *SF) {}

/// \return true if syntax analysis is successful
Status stone::PerformScaffolding(FrontendModule &fm,
                                 FrontendModuleCallback callback) {

  Status::Success();
}

/// \return true if syntax analysis is successful
Status stone::PerformScaffolding(SourceFile *SF) { Status::Success(); }

/// \return true if syntax analysis is successful
Status stone::PerformCheckTypes(FrontendModule &fm,
                                FrontendModuleCallback callback) {

  Status::Success();
}

/// \return true if syntax analysis is successful
Status stone::PerformCheckTypes(SourceFile *S) { Status::Success(); }

// \return true if syntax analysis is successful
Status stone::PerformCodeGen(FrontendModule &fm,
                             FrontendModuleCallback callback) {

  Status::Success();
}

// \return true if syntax analysis is successful
Status stone::PerformBackend(FrontendModule &fm,
                             FrontendModuleCallback callback) {

  Status::Success();
}
