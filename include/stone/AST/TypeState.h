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
  Type *canonicalType = nullptr;

public:
  explicit TypeState(ASTContext &astContext) : astContext(astContext) {}

public:
  Type *GetCanonicalType() const { return canonicalType; }
  void SetCanonicalType(Type *CT) {
    assert(CT && "TypeState cannot be assigned a null Type!");
    canonicalType = CT;
  }
  bool IsCanonical() const { return GetCanonicalType() != nullptr; }
};

class BuiltinTypeState final : public TypeState {

public:
  BuiltinTypeState(ASTContext &astContext) : TypeState(astContext) {}
};

} // namespace stone

#endif