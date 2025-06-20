#ifndef STONE_FRONTEND_FRONTEND_OBSERVER_H
#define STONE_FRONTEND_FRONTEND_OBSERVER_H

#include "llvm/ADT/ArrayRef.h"
#include "llvm/IR/Module.h"

namespace stone {

class SourceFile;
class SpaceDecl;
class ParserObserver;
class LogicObserver;
class CodeGenObserver;
class Frontend;

class FrontendObserver {

  ParserObserver *parserObserver = nullptr;
  LogicObserver *typeCheckerObserver = nullptr;
  CodeGenObserver *codeGenObserver = nullptr;

public:
  FrontendObserver() {}
  virtual ~FrontendObserver() = default;

public:
  /// The command line has been parsed.
  virtual void CompletedParsingOptions(const Frontend &frontend) {}

  /// Completed the creation of SpaceDecl
  virtual void CompletedSpaceDecl(const SpaceDecl *result) {}

public:
  // Set the parser observer
  void SetParserObserver(ParserObserver *observer) {
    parserObserver = observer;
  }
  ///\return the ParserObserver
  ParserObserver *GetParserObserver() { return parserObserver; }

  ///\return true if there exist a CodeCompletionCallbacks
  bool HasParserObserver() { return GetParserObserver() != nullptr; }

public:
  // Set the Logic observer
  void SetLogicObserver(LogicObserver *observer) {
    typeCheckerObserver = observer;
  }

  /// Callbacks into the parsing pipeline
  LogicObserver *GetLogicObserver() { return typeCheckerObserver; }

  ///\return ture if there exist a LogicObserver
  bool HasLogicObserver() { return GetLogicObserver() != nullptr; }

public:
  // Callbacks into the parsing pipeline
  void SetCodeGenObserver(CodeGenObserver *observer) {
    codeGenObserver = observer;
  }

  /// Callbacks into the parsing pipeline
  CodeGenObserver *GetCodeGenObserver() { return codeGenObserver; }

  ///\return true if there exist a CodeCompletionCallbacks
  bool HasCodeGenObserver() { return GetCodeGenObserver() != nullptr; }
};

} // namespace stone

#endif