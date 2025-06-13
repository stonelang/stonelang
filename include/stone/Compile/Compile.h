#ifndef STONE_COMPILE_COMPILE_H
#define STONE_COMPILE_COMPILE_H

#include "stone/Support/Status.h"

#include "llvm/ADT/ArrayRef.h"
#include "llvm/IR/Module.h"

namespace stone {

class SourceFile;
class Frontend;
class FrontendModule;
class FrontendObserver;
using FrontendModuleCallback = std::function<bool(FrontendModule &fm)>;

int Compile(llvm::ArrayRef<const char *> args, const char *arg0, void *mainAddr,
            FrontendObserver *observer = nullptr);

/// \return a frontend module
FrontendModule *CreateFrontendModule(Frontend &frontend);

/// \return status of compile
Status PerformCompile(FrontendModule &frontendModule);

/// \return true if we compiled an ir file.
Status PerformCompileLLVM(FrontendModule &frontendModule);

/// \return true if syntax analysis is successful
Status PerformParsing(FrontendModule &frontendModule,
                      FrontendModuleCallback callback);

/// \return true if syntax analysis is successful
Status PerformParsing(SourceFile *SF);

/// \return true if syntax analysis is successful
Status PerformScaffolding(FrontendModule &frontendModule,
                          FrontendModuleCallback callback);

/// \return true if syntax analysis is successful
Status PerformTypeChecking(FrontendModule &frontendModule,
                           FrontendModuleCallback callback);

// \return true if syntax analysis is successful
Status PerformCodeGen(FrontendModule &frontendModule,
                      FrontendModuleCallback callback);

// \return true if syntax analysis is successful
Status PerformBackend(FrontendModule &frontendModule,
                      FrontendModuleCallback callback);

} // namespace stone

#endif
