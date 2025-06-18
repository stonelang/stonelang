#ifndef STONE_COMPILE_COMPILE_H
#define STONE_COMPILE_COMPILE_H

#include "stone/Support/Status.h"

#include "llvm/ADT/ArrayRef.h"
#include "llvm/IR/Module.h"

namespace stone {
class Frontend;
class FrontendModule;
class FrontendObserver;

int Compile(llvm::ArrayRef<const char *> args, const char *arg0, void *mainAddr,
            FrontendObserver *observer = nullptr);

// /// \return a frontend module
// FrontendSpace *CreateFrontendSpace(Frontend &frontend);

// /// \return status of compile
// Status PerformCompile(FrontendSpace &fm);

// /// \return Success if we compiled an ir file.
// Status PerformCompileLLVM(FrontendSpace &fm);

// /// \return Success if syntax analysis is successful
// Status PerformParse(FrontendSpace &fm);

// /// \return Success if syntax analysis is successful
// Status PerformParse(SourceFile *SF);

// // \return true if syntax analysis is successful
// Status PerformScaffolding(FrontendSpace &fm);

// /// \return true if syntax analysis is successful
// Status PerformScaffolding(SourceFile *S);

// /// \return true if syntax analysis is successful
// Status PerformCheckTypes(FrontendSpace &fm);

// /// \return true if syntax analysis is successful
// Status PerformCheckTypes(SourceFile *S);

// // \return true if syntax analysis is successful
// Status PerformCodeGen(FrontendSpace &fm);

// // \return true if syntax analysis is successful
// Status PerformBackend(FrontendSpace &fm);

} // namespace stone

#endif
