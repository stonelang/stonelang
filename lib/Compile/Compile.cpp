#include "stone/Compile/Compile.h"
#include "stone/Compile/FrontendModule.h"
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
  if (frontend.ParseArgStrings(args).IsError()) {
  }
  frontend.SetObserver(observer);

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

  // Simple for now
  if (!frontend.SetupASTSession().IsSuccess()) {
    return 0;
  }
  FrontendModule *frontendModule = stone::CreateFrontendModule(frontend);
  if (!stone::PerformCompile(*frontendModule).IsSuccess()) {
    return 0;
  }
}

FrontendModule *stone::CreateFrontendModule(Frontend &frontend) {
  return new (frontend.GetASTSession()) FrontendModule(frontend);
}
/// \return status of compile
Status stone::PerformCompile(FrontendModule &frontendModule) {
  Status::Success();
}

/// \return true if we compiled an ir file.
Status stone::PerformCompileLLVM(FrontendModule &frontendModule) {
  Status::Success();
}

/// \return true if syntax analysis is successful
Status stone::PerformParsing(FrontendModule &frontendModule,
                             FrontendModuleCallback callback) {

  // when we parse, we add to the mdou

  // frontendModule.GetModule().AddSourceFile()
  Status::Success();
}

// \return true if syntax analysis is successful
Status stone::PerformParsing(SourceFile *SF) {}

/// \return true if syntax analysis is successful
Status stone::PerformScaffolding(FrontendModule &frontendModule,
                                 FrontendModuleCallback callback) {

  Status::Success();
}

/// \return true if syntax analysis is successful
Status stone::PerformTypeChecking(FrontendModule &frontendModule,
                                  FrontendModuleCallback callback) {

  Status::Success();
}

// \return true if syntax analysis is successful
Status stone::PerformCodeGen(FrontendModule &frontendModule,
                             FrontendModuleCallback callback) {

  Status::Success();
}

// \return true if syntax analysis is successful
Status stone::PerformBackend(FrontendModule &frontendModule,
                             FrontendModuleCallback callback) {

  Status::Success();
}
