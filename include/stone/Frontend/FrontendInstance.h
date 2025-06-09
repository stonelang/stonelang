#ifndef STONE_FRONTEND_FRONTENDINSTANCE_H
#define STONE_FRONTEND_FRONTENDINSTANCE_H

#include "stone/AST/ASTContext.h"
#include "stone/AST/Module.h"
#include "stone/Frontend/FrontendInvocation.h"

namespace stone {

class FrontendInstance final {

  FrontendInvocation &invocation;
  std::unique_ptr<ASTContext> astContext;

  /// If there is no stats output directory by the time the
  /// instance has completed its setup, this will be null.
  // std::unique_ptr<StatsReporter> stats;

  /// Contains buffer IDs for input source code files.
  std::vector<unsigned> srcIDList;

  mutable ModuleDecl *frontendModule = nullptr;

public:
  FrontendInstance(const FrontendInstance &) = delete;
  void operator=(const FrontendInstance &) = delete;

public:
  FrontendInstance(FrontendInvocation &invocation);
  bool Setup();

public:
  ModuleDecl *GetFrontendModule() { return frontendModule; }
  void SetFrontendModule(ModuleDecl *frontendModule);
};

} // namespace stone

#endif // STONE_FRONTEND_FRONTENDINPUTFILE_H
