#include "stone/Compile/Compile.h"
#include "stone/Compile/FrontendObserver.h"

using namespace stone;

int stone::Compile(llvm::ArrayRef<const char *> args, const char *arg0,
                   void *mainAddr, FrontendObservation *observation) {

  return 0;
}
/// \return status of compile
Status stone::Compile(Compiling &compiling) { Status::Success(); }

/// \return true if we compiled an ir file.
Status stone::PerformCompileLLVM(Compiling &compiling) { Status::Success(); }

/// \return true if syntax analysis is successful
Status stone::PerformParsing(Compiling &compiling, CompilingCallback callback) {

  // when we parse, we add to the mdou

  // compiling.GetModule().AddSourceFile()
  Status::Success();
}

// \return true if syntax analysis is successful
Status stone::PerformParsing(SourceFile *SF) {}

/// \return true if syntax analysis is successful
Status stone::PerformScaffolding(Compiling &compiling,
                                 CompilingCallback callback) {

  Status::Success();
}

/// \return true if syntax analysis is successful
Status stone::PerformTypeChecking(Compiling &compiling,
                                  CompilingCallback callback) {

  Status::Success();
}

// \return true if syntax analysis is successful
Status stone::PerformCodeGen(Compiling &compiling, CompilingCallback callback) {

  Status::Success();
}

// \return true if syntax analysis is successful
Status stone::PerformBackend(Compiling &compiling, CompilingCallback callback) {

  Status::Success();
}
