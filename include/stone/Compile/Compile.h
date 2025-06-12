#ifndef STONE_COMPILE_COMPILE_H
#define STONE_COMPILE_COMPILE_H

#include "llvm/ADT/ArrayRef.h"
#include "llvm/IR/Module.h"

namespace stone {

class Compiling;
class SourceFile;
class FrontendObservation;
using CompilingCallback = std::function<bool(Compiling &compiling)>;

int Compile(llvm::ArrayRef<const char *> args, const char *arg0, void *mainAddr,
            FrontendObservation *observation = nullptr);

/// \return status of compile
Status Compile(Compiling &compiling);

/// \return true if we compiled an ir file.
Status PerformCompileLLVM(Compiling &compiling);

/// \return true if syntax analysis is successful
Status PerformParsing(Compiling &compiling, CompilingCallback callback);

/// \return true if syntax analysis is successful
Status PerformParsing(SourceFile *SF);

/// \return true if syntax analysis is successful
Status PerformScaffolding(Compiling &compiling, CompilingCallback callback);

/// \return true if syntax analysis is successful
Status PerformTypeChecking(Compiling &compiling, CompilingCallback callback);

// \return true if syntax analysis is successful
Status PerformCodeGen(Compiling &compiling, CompilingCallback callback);

// \return true if syntax analysis is successful
Status PerformBackend(Compiling &compiling, CompilingCallback callback);

} // namespace stone

#endif
