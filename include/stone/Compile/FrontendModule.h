#ifndef STONE_COMPILE_FRONTEND_MODULE_H
#define STONE_COMPILE_FRONTEND_MODULE_H

#include "stone/AST/ModuleDecl.h"
#include "stone/Compile/Frontend.h"

namespace stone {

class FrontendModule final : public NormalModuleDecl {
  Frontend &frontend;

public:
  FrontendModule(const FrontendModule &) = delete;
  void operator=(const FrontendModule &) = delete;

public:
  explicit FrontendModule(Frontend &frontend);

public:
  Frontend &GetFronend() { return frontend; }
};
} // namespace stone

#endif // STONE_FRONTEND_FRONTENDINPUTFILE_H
