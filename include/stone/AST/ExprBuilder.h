#ifndef STONE_AST_EXPR_BUILDER_H
#define STONE_AST_EXPR_BUILDER_H

#include <stdint.h>

namespace stone {
class ASTSession;

class ExprBuilder final {
  const ASTSession &session;

public:
  ExprBuilder(const ASTSession &session);

public:
};

} // namespace stone

#endif
