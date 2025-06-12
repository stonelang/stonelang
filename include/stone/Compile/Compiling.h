#ifndef STONE_COMPILE_COMPILING_H
#define STONE_COMPILE_COMPILING_H

#include "stone/AST/ASTSession.h"
#include "stone/AST/ModuleDecl.h"
#include "stone/Compile/Frontend.h"

namespace stone {

class Compiling final {

  std::unique_ptr<Frontend> frontend;
  std::unique_ptr<ASTSession> session;

  mutable ModuleDecl *normalModule = nullptr;
  FrontendObserver *observer = nullptr;

public:
  Compiling(const Compiling &) = delete;
  void operator=(const Compiling &) = delete;
  explicit Compiling(std::unique_ptr<Frontend> frontend);
  bool Setup();

public:
  void SetFrontendObserver(FrontendObserver *obs) { observer = obs; }
  FrontendObserver *GetFrontendObserver() { return observer; }
  ASTSession &GetASTSession() { return *session; }
  ModuleDecl *GetModule() { return normalModule; }

public:
  SourceFile *CreateSourceFile(const InputFile inputFile);
};

} // namespace stone

#endif // STONE_FRONTEND_FRONTENDINPUTFILE_H
