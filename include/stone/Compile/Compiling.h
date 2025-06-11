#ifndef STONE_COMPILE_COMPILING_H
#define STONE_COMPILE_COMPILING_H

#include "stone/AST/ASTSession.h"
#include "stone/AST/ModuleDecl.h"
#include "stone/Compile/Frontend.h"

namespace stone {

class Compiling final {

  std::unique_ptr<Frontend> frontend;
  mutable ModuleDecl *mod = nullptr;
  FrontendObserver *observer = nullptr;

public:
  Compiling(const Compiling &) = delete;
  void operator=(const Compiling &) = delete;
  explicit Compiling(std::unique_ptr<Frontend> frontend);

public:
  bool Setup();
  void SetFrontendObserver(FrontendObserver *obs) { observer = obs; }
  FrontendObserver *GetFrontendObserver() { return observer; }

public:
  ModuleDecl *GetModule() { return mod; }
  void SetModule(ModuleDecl *mod);
  Frontend &GetFronend() { return *frontend; }
};

} // namespace stone

#endif // STONE_FRONTEND_FRONTENDINPUTFILE_H
