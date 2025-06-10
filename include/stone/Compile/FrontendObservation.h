#ifndef STONE_COMPILE_COMPILER_OBSERVATION_H
#define STONE_COMPILE_COMPILER_OBSERVATION_H

#include "llvm/ADT/ArrayRef.h"
#include "llvm/IR/Module.h"

namespace stone {

class SourceFile;
class ModuleDecl;
class CodeCompletionCallbacks;

class FrontendObservation {
public:
  FrontendObservation() {}
  virtual ~FrontendObservation() = default;

public:
  /// The command line has been parsed.
  virtual void CompletedCommandLineParsing(const Frontend &frontend) {}

  /// Completed syntax analysis
  virtual void CompletedSyntaxAnalysis(const Frontend &frontend,
                                       const SourceFile &result) {}

  /// Completed syntax analysis
  virtual void CompletedSyntaxAnalysis(const Frontend &frontend,
                                       const ModuleDecl &result) {}

  /// Completed semantic analysis
  virtual void CompletedSemanticAnalysis(const Frontend &frontend,
                                         const SourceFile &result) {}

  /// Completed semantic analysis
  virtual void CompletedSemanticAnalysis(const Frontend &frontend,
                                         const ModuleDecl &result) {}

  // /// Some executions may require access to the results of ir generation.
  virtual void CompletedIRGeneration(const Frontend &frontend,
                                     const llvm::Module *result) {}

  /// Callbacks into the parsing pipeline
  virtual CodeCompletionCallbacks *GetCodeCompletionCallbacks() {
    return nullptr;
  }

  ///\return true if there exist a CodeCompletionCallbacks
  bool HasCodeCompletionCallbacks() {
    return GetCodeCompletionCallbacks() != nullptr;
  }
};

} // namespace stone

#endif