#ifndef STONE_AST_TYPE_BUILDER_H
#define STONE_AST_TYPE_BUILDER_H

#include <stdint.h>

namespace stone {
class ASTSession;

class TypeBuilder final {
  const ASTSession &session;

public:
  TypeBuilder(const ASTSession &session);

public:
};

} // namespace stone

#endif
