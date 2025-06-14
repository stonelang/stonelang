#ifndef STONE_AST_TYPESTATE_H
#define STONE_AST_TYPESTATE_H

#include "stone/AST/ASTAllocation.h"
#include "stone/AST/TypeAlignment.h"
#include "stone/AST/TypeInfluencer.h"

namespace stone {

class DeclState;

class alignas(1 << TypeAlignInBits) TypeState
    : public ASTAllocation<TypeState> {

  friend class ASTSession;
  ASTSession &session;

  // The type that owns this TypeState
  ///\ If the owner type is null
  Type *owner = nullptr;

  // The type that owns this TypeState
  ///\ If this is cannonical
  Type *canType = nullptr;

  // Source location of the type
  SrcLoc typeLoc;

  // The DeclState that owns this TypeState
  DeclState *declState = nullptr;

public:
  explicit TypeState(ASTSession &session) : session(session) {}

public:
  ASTSession &GetASTSession() { return session; }

  void SetCanType(Type *T) {
    assert(T && "TypeState cannot be assigned a null Type!");
    canType = T;
  }
  Type *GetCanType() { return canType; }
  bool HasCanType() { return canType != nullptr; }
  bool HasName();

  void SetOwner(Type *ty) { owner = ty; }
  Type *GetOwner() { return owner; }
  bool HasOwner() { return owner != nullptr; }

  void SetLoc(SrcLoc loc) { typeLoc = loc; }
  SrcLoc GetLoc() { return typeLoc; }
  bool HasLoc() { return typeLoc.isValid(); }
};

class BuiltinTypeState final : public TypeState {

public:
  BuiltinTypeState(ASTSession &session) : TypeState(session) {}
};

} // namespace stone

#endif