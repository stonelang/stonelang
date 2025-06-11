#ifndef STONE_AST_ASTBUILDER_H
#define STONE_AST_ASTBUILDER_H

#include <stdint.h>

namespace stone {
class FunDecl;
class ASTSession;

class ASTBuilder final {
  const ASTSession &session;

public:
  ASTBuilder(const ASTSession &session);

public:
  FunDecl *CreateFunDecl();
}
};

} // namespace stone

#endif
