#ifndef STONE_FRONTEND_FRONTEND_MODULE_H
#define STONE_FRONTEND_FRONTEND_MODULE_H

#include "stone/AST/ModuleDecl.h"
#include "stone/Frontend/Frontend.h"
#include "stone/Core/Status.h"

namespace stone {

// class ModuleManager {
//   llvm::StringMap<std::unique_ptr<FrontendModule>> modules;
//   Memory sharedMemory;

//   ModuleContext makeContext() {
//     return {
//       sharedMemory,
//       /* sharedIdentifiers */ ...,
//       /* sharedBuiltins */ ...
//     };
//   }
// };

class FrontendModule final {

  const Frontend &frontend;

  Module *underlyingModule;
  std::unique_ptr<Memory> memoryManager;

public:
  FrontendModule(const FrontendModule &) = delete;
  void operator=(const FrontendModule &) = delete;

public:
  explicit FrontendModule(const Frontend &frontend);

public:
  Frontend &GetFrontend() { return frontend; }
  Module *GetPointer() { return underlyingModule; }

public:
  void PlanEvaluators();
  void Evaluate();
  void Evaluate(EvaluatorKind kind);
};
} // namespace stone

#endif // STONE_FRONTEND_FRONTENDINPUTFILE_H