#ifndef STONE_FRONTEND_FRONTEND_MODULE_H
#define STONE_FRONTEND_FRONTEND_MODULE_H

#include "stone/AST/ModuleDecl.h"
#include "stone/Frontend/Frontend.h"
#include "stone/Support/Status.h"

namespace stone {

// class ModuleManager {
//   llvm::StringMap<std::unique_ptr<FrontendModule>> modules;
//   MemoryManager sharedMemory;

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
  std::unique_ptr<MemoryManager> memoryManager;

public:
  FrontendModule(const FrontendModule &) = delete;
  void operator=(const FrontendModule &) = delete;

public:
  explicit FrontendModule(const Frontend &frontend);

public:
  Frontend &GetFrontend() { return frontend; }
  Module *GetPointer() { return underlyingModule; }
};
} // namespace stone

#endif // STONE_FRONTEND_FRONTENDINPUTFILE_H