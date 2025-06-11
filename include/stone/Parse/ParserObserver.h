#ifndef STONE_PARSE_CODECOMPLETION_CALLBACKS_H
#define STONE_PARSE_CODECOMPLETION_CALLBACKS_H

#include "llvm/ADT/StringRef.h"

namespace stone {
class Parser;
class SourceFile;
class Token;
class Decl;

class ParserObserver {

public:
  ParserObserver() {}
  virtual ~ParserObserver() = default;

public:
  virtual void CompletedToken(Token token) {}
  virtual void CompletedDecl(const Decl *decl) {}
  virtual void CompletedExpr(const Expr *expr) {}
  virtual void CompletedStmt(const Expr *expr) {}
  virtual void CompletedSourceFile(const SourceFile *srcFile) {}
};

} // namespace stone
#endif
