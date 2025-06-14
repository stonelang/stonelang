#ifndef STONE_FRONTEND_FRONTEND_MODULE_H
#define STONE_FRONTEND_FRONTEND_MODULE_H

#include "stone/AST/ModuleDecl.h"
#include "stone/Frontend/Frontend.h"
#include "stone/Support/Status.h"

namespace stone {

class FrontendModule final : public NormalModuleDecl {
  Frontend &frontend;

public:
  FrontendModule(const FrontendModule &) = delete;
  void operator=(const FrontendModule &) = delete;

public:
  explicit FrontendModule(Frontend &frontend);
  Status Initialize();

public:
  Frontend &GetFrontend() { return frontend; }
};
} // namespace stone

#endif // STONE_FRONTEND_FRONTENDINPUTFILE_H
