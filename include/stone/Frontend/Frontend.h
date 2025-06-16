#ifndef STONE_FRONTEND_FRONTEND_H
#define STONE_FRONTEND_FRONTEND_H

#include "stone/AST/ASTSession.h"
#include "stone/AST/TypeCheckerOptions.h"
#include "stone/Frontend/FrontendMemoryBuffers.h"
#include "stone/Frontend/FrontendObserver.h"
#include "stone/Frontend/FrontendOptions.h"
#include "stone/Support/CodeGenOptions.h"
#include "stone/Support/DiagnosticOptions.h"
#include "stone/Support/LangOptions.h"
#include "stone/Support/Status.h"

#include "clang/Basic/FileManager.h"
#include "clang/Basic/FileSystemOptions.h"

#include "llvm/ADT/ArrayRef.h"
#include "llvm/ADT/SmallVector.h"
#include "llvm/Option/ArgList.h"
#include <memory>

namespace stone {

class FrontendArgListConverter final {
public:
  FrontendArgListConverter();

public:
  Status Convert(Frontend &frontend);
  Status ParseFrontendOptions();
  Status ParseFrontendMode();
  Status ParseDiagnosticOptions();
  Status ParseTargetOptions();
  Status ParseCodeGenOptions();
};

class FrontendCommandLine {
  friend FrontendArgListConverter;

  FrontendArgListConverter converter;
  FrontendArgListConverter &GetArgListConverter() { return converter; }

public:
  FrontendCommandLine() {}

public:
  Status ParseArgStrings(llvm::ArrayRef<const char *> args);
};

class Frontend final {
  friend FrontendArgListConverter;

  FrontendOptions frontendOpts;
  CodeGenOptions codeGenOpts;
  TypeCheckerOptions typeCheckerOpts;
  DiagnosticOptions diagOpts;
  clang::FileSystemOptions clangFileSystemOpts;
  std::unique_ptr<llvm::opt::InputArgList> inputArgList;
  FrontendObserver *observer;

  std::unique_ptr<ASTSession> session;
  std::unique_ptr<ClangImporter> clangImporter;

  std::unique_ptr<ASTMemory> astMemory;
  Space *space = nullptr;

  FrontendArgListConverter converter;
  FrontendArgListConverter &GetArgListConverter() { return converter; }

public:
  Frontend(llvm::StringRef executablePath, llvm::StringRef executableNam);
  Status ParseArgStrings(llvm::ArrayRef<const char *> args);
  FrontendMode GetMode() { return frontendOpts.Mode; }

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

public:
  Status SetupASTSession();
  bool HasASTSession() { return session != nullptr; }
  ASTSession &GetASTSession() { return *session; }

  // llvm::ArrayRef<EvaluatorKind> GetPipeline(FrontendMode mode);
  FrontendSpace *CreateSpace();

private:
  Status SetupClang(llvm::ArrayRef<const char *> args, const char *arg0);
};

} // namespace stone

#endif // STONE_FRONTEND_FRONTENDINPUTFILE_H
