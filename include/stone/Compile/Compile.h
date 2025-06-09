#ifndef STONE_COMPILE_COMPILE_H
#define STONE_COMPILE_COMPILE_H

#include "llvm/ADT/ArrayRef.h"
#include "llvm/IR/Module.h"

namespace stone {

class FrontendInstance;
class FrontendObservation;

int Compile(llvm::ArrayRef<const char *> args, const char *arg0, void *mainAddr,
            FrontendObservation *observation = nullptr);

/// \return true if compilie is successful
bool PerformCompile(FrontendInstance &instance);

/// \retyrb true if we compiled an ir file.
bool PerformCompileLLVM(FrontendInstance &instance);

/// \return true if syntax analysis is successful
bool PerformParse(FrontendInstance &instance,
                  std::function<bool(FrontendInstance &)>);

} // namespace stone

#endif
