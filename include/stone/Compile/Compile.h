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
using FrontendModuleCallback = std::function<Status(FrontendModule &fm)>;

int Compile(llvm::ArrayRef<const char *> args, const char *arg0, void *mainAddr,
            FrontendObserver *observer = nullptr);

/// \return a frontend module
FrontendModule *CreateFrontendModule(Frontend &frontend);

/// \return status of compile
Status PerformCompile(FrontendModule &fm);

/// \return Success if we compiled an ir file.
Status PerformCompileLLVM(FrontendModule &fm);

/// \return Success if syntax analysis is successful
Status PerformParse(FrontendModule &fm, FrontendModuleCallback cb);

/// \return Success if syntax analysis is successful
Status PerformParse(SourceFile *SF);

// \return true if syntax analysis is successful
Status PerformScaffolding(FrontendModule &fm, FrontendModuleCallback cb);

/// \return true if syntax analysis is successful
Status PerformScaffolding(SourceFile *S);

/// \return true if syntax analysis is successful
Status PerformCheckTypes(FrontendModule &fm, FrontendModuleCallback cb);

/// \return true if syntax analysis is successful
Status PerformCheckTypes(SourceFile *S);

// \return true if syntax analysis is successful
Status PerformCodeGen(FrontendModule &fm, FrontendModuleCallback cb);

// \return true if syntax analysis is successful
Status PerformBackend(FrontendModule &fm, FrontendModuleCallback cb);

} // namespace stone

#endif
