#include "stone/Compile/Compile.h"
#include "stone/Compile/FrontendObserver.h"

using namespace stone;

int stone::Compile(llvm::ArrayRef<const char *> args, const char *arg0,
                   void *mainAddr, FrontendObservation *observation) {

  return 0;
}
/// \return status of compile
Status stone::Compile(Compiling &compiling) {}

/// \return true if we compiled an ir file.
Status stone::PerformCompileLLVM(Compiling &compiling) {}

/// \return true if syntax analysis is successful
Status stone::PerformParse(Compiling &compiling,
                           std::function<bool(Compiling &compiling)>) {}
