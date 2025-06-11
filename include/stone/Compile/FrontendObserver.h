#ifndef STONE_COMPILE_FRONTEND_OBSERVER_H
#define STONE_COMPILE_FRONTEND_OBSERVER_H

#include "llvm/ADT/ArrayRef.h"
#include "llvm/IR/Module.h"

namespace stone {

class SourceFile;
class ModuleDecl;
class ParserObserver;
class TypeCheckingObserver;
class CodeGenObserver;

class FrontendObserver {

  ParserObserver *parserObserver = nullptr;
  TypeCheckerObserver *typeCheckerObserver = nullptr;
  CodeGenObserver *codeGenObserver = nullptr;

public:
  FrontendObserver() {}
  virtual ~FrontendObserver() = default;

public:
  /// The command line has been parsed.
  virtual void CompletedParsingOptions(const Frontend &frontend) {}

  /// Completed the creation of ModuleDecl
  virtual void CompletedModuleDecl(const ModuleDecl *result) {}

public:
  // Set the parser observer
  void SetParserObserver(ParserObserver *observer) {
    return parserObserver = observer;
  }
  ///\return the ParserObserver
  ParserObserver *GetParserObserver() { return parserObserver; }

  ///\return true if there exist a CodeCompletionCallbacks
  bool HasParserObserver() { return GetParserObserver() != nullptr; }

public:
  // Set the TypeChecker observer
  void SetTypeCheckerObserver(TypeCheckerObserver *observer) {
    return typeCheckerObserver = observer;
  }

  /// Callbacks into the parsing pipeline
  TypeCheckerObserver *GetTypeCheckerObserver() { return typeCheckerObserver; }

  ///\return ture if there exist a TypeCheckerObserver
  bool HasTypeCheckerObserver() { return GetTypeCheckerObserver() != nullptr; }

public:
  // Callbacks into the parsing pipeline
  void SetCodeGenObserver(CodeGenObserver *observer) {
    return codeGenObserver = observer;
  }

  /// Callbacks into the parsing pipeline
  CodeGenObserver *GetCodeGenObserver() { return codeGenObserver; }

  ///\return true if there exist a CodeCompletionCallbacks
  bool HasCodeGenObserver() { return GetCodeGenObserver() != nullptr; }
};

} // namespace stone

#endif