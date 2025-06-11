#ifndef STONE_AST_ASTBUILDER_H
#define STONE_AST_ASTBUILDER_H

#include <stdint.h>

namespace stone {
class FunDecl;
class ModuleDecl;
class ASTSession;

class ASTBuilder final {
  const ASTSession &session;

public:
  ASTBuilder(const ASTSession &session);

public:
  ModuleDecl *CreateModuleDecl();
  FunDecl *CreateFunDecl();

public:
};

} // namespace stone

#endif
