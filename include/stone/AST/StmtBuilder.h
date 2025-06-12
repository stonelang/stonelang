#ifndef STONE_AST_STMT_BUILDER_H
#define STONE_AST_STMT_BUILDER_H

#include <stdint.h>

namespace stone {
class ASTSession;

class StmtBuilder final {
  const ASTSession &session;

public:
  StmtBuilder(const ASTSession &session);

public:
};

} // namespace stone

#endif
