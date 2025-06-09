#ifndef STONE_AST_TYPESTATE_H
#define STONE_AST_TYPESTATE_H

#include "stone/AST/AST.h"
#include "stone/AST/TypeAlignment.h"
#include "stone/AST/TypeInfluencer.h"

namespace stone {

class alignas(1 << TypeAlignInBits) TypeState
    : public ASTAllocation<TypeState> {

  friend class ASTContext;
  ASTContext &astContext;

  // The canonical type associated with this TypeSate
  Type *canType = nullptr;

public:
  explicit TypeState(ASTContext &astContext) : astContext(astContext) {}

public:
  Type *GetCanType() const { return canType; }
  void SetCanType(Type *ty) {
    assert(ty && "TypeState cannot be assigned a null Type!");
    canType = ty;
  }
};

class BuiltinTypeState final : public TypeState {

public:
  BuiltinTypeState(ASTContext &astContext) : TypeState(astContext) {}
};

} // namespace stone

#endif