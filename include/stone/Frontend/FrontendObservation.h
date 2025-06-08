#ifndef STONE_COMPILE_COMPILER_OBSERVATION_H
#define STONE_COMPILE_COMPILER_OBSERVATION_H

#include "llvm/ADT/ArrayRef.h"
#include "llvm/IR/Module.h"

namespace stone {

class CodeCompletionCallbacks;
class FrontendObservation {
public:
  FrontendObservation() {}
  virtual ~FrontendObservation() = default;

public:
  /// The command line has been parsed.
  // virtual void CompletedCommandLineParsing(FrontendInstance &result) {}

  // /// The compiler has been configured
  // virtual void CompletedConfiguration(FrontendInstance &result) {}

  // /// Completed syntax analysis
  // virtual void CompletedSyntaxAnalysis(SourceFile &result) {}

  // /// Completed syntax analysis
  // virtual void CompletedSyntaxAnalysis(ModuleDecl &result) {}

  // /// Completed syntax analysis
  // virtual void CompletedSyntaxAnalysis(FrontendInstance &result) {}

  // /// Completed semantic analysis
  // virtual void CompletedSemanticAnalysis(SourceFile &result) {}

  // /// Completed semantic analysis
  // virtual void CompletedSemanticAnalysis(ModuleDecl &result) {}

  // /// Completed semantic analysis
  // virtual void CompletedSemanticAnalysis(FrontendInstance &result) {}

  // /// Some executions may require access to the results of ir generation.
  // virtual void CompletedIRGeneration(llvm::Module *result) {}

  // /// Some executions may require access to the results of ir generation.
  // virtual void CompletedIRGeneration(llvm::ArrayRef<llvm::Module *> &results)
  // {}

  // // Completed IR generation
  // virtual void CompletedIRGeneration(FrontendInstance &result) {}

  // /// Callbacks into the parsing pipeline
  // virtual CodeCompletionCallbacks *GetCodeCompletionCallbacks() {}

  // ///\return true if there exist a CodeCompletionCallbacks
  // bool HasCodeCompletionCallbacks() {
  //   return GetCodeCompletionCallbacks() != nullptr;
  // }
};

} // namespace stone

#endif