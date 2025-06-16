#ifndef STONE_FRONTEND_FRONTEND_MODULE_H
#define STONE_FRONTEND_FRONTEND_MODULE_H

#include "stone/AST/SpaceDecl.h"
#include "stone/Frontend/Frontend.h"
#include "stone/Support/Status.h"

namespace stone {

class FrontendSpace final : public NormalSpaceDecl {
  Frontend &frontend;

public:
  FrontendSpace(const FrontendSpace &) = delete;
  void operator=(const FrontendSpace &) = delete;

public:
  explicit FrontendSpace(Frontend &frontend);
  Status Initialize();
  Status Evaluate();

public:
  Frontend &GetFrontend() { return frontend; }
};
} // namespace stone

#endif // STONE_FRONTEND_FRONTENDINPUTFILE_H
