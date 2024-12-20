#include "stone/Compile/Compile.h"
#include "stone/Frontend/Frontend.h"

int stone::Compile(llvm::ArrayRef<const char *> args, const char *arg0,
                   void *mainAddr, FrontendObservation *observation) {

}

bool stone::PerformCompile(FrontendInstance &instance) {}

bool stone::PerformParse(FrontendInstance &instance) {}

bool stone::PerformParse(FrontendInstance &instance, SourceFile &sourceFile) {}

bool stone::PerformEmitParse(FrontendInstance &instance) {}

bool stone::PerformResolveImports(FrontendInstance &instance) {}

bool stone::PerformResolveImports(FrontendInstance &instance,
                                  SourceFile &sourceFile) {}

bool stone::PerformSemanticAnalysis(FrontendInstance &instance) {}

bool stone::PerformSemanticAnalysis(FrontendInstance &instance,
                                    SourceFile &sourceFile) {}

bool stone::PerformEmitAST(FrontendInstance &instance) {}

bool stone::CompletedSemanticAnalysis(FrontendInstance &instance) {}


bool stone::PerformCompileLLVM(FrontendInstance &instance) {}