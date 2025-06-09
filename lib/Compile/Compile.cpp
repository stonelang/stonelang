#include "stone/Compile/Compile.h"
#include "stone/Frontend/FrontendObservation.h"

using namespace stone;

int stone::Compile(llvm::ArrayRef<const char *> args, const char *arg0,
                   void *mainAddr, FrontendObservation *observation) {

  return 0;
}

/// \return true if compilie is successful
bool stone::PerformCompile(FrontendInstance &instance) { return false; }

/// \retyrb true if we compiled an ir file.
bool stone::PerformCompileLLVM(FrontendInstance &instance) { return false; }

/// \return true if syntax analysis is successful
bool stone::PerformParse(FrontendInstance &instance,
                         std::function<bool(FrontendInstance &)>) {

  // instance.GetFrontendModule().AddSource(sourcefe);
}
