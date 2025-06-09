#ifndef STONE_AST_TYPESTATE_H
#define STONE_AST_TYPESTATE_H

#include "stone/AST/AST.h"
#include "stone/AST/TypeAlignment.h"

namespace stone {

class alignas(1 << TypeAlignInBits) TypeState
    : public ASTAllocation<TypeState> {

  friend class ASTContext;
  const ASTContext &astContext;

public:
  explicit TypeState(const ASTContext &astContext) : astContext(astContext) {}
};

class BuiltinTypeState final : public TypeState {

public:
  BuiltinTypeState(const ASTContext &astContext) : TypeState(astContext) {}
};

} // namespace stone

#endif