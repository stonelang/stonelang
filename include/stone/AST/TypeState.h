#ifndef STONE_AST_TYPESTATE_H
#define STONE_AST_TYPESTATE_H

#include "stone/AST/ASTAllocation.h"
#include "stone/AST/TypeAlignment.h"
#include "stone/AST/TypeInfluencer.h"

namespace stone {

class alignas(1 << TypeAlignInBits) TypeState
    : public ASTAllocation<TypeState> {

  friend class ASTSession;
  ASTSession &session;
  Type *canonicalType = nullptr;

public:
  explicit TypeState(ASTSession &session) : session(session) {}

public:
  Type *GetCanonicalType() const { return canonicalType; }
  void SetCanonicalType(Type *CT) {
    assert(CT && "TypeState cannot be assigned a null Type!");
    canonicalType = CT;
  }
  bool IsCanonical() const { return GetCanonicalType() != nullptr; }
  ASTSession &GetASTSession() { return session; }
};

class BuiltinTypeState final : public TypeState {

public:
  BuiltinTypeState(ASTSession &session) : TypeState(session) {}
};

} // namespace stone

#endif