#ifndef STONE_AST_TYPESTATE_H
#define STONE_AST_TYPESTATE_H

#include "stone/AST/Artifact.h"
#include "stone/AST/MemoryAllocation.h"
#include "stone/AST/TypeAlignment.h"
#include "stone/AST/TypeInfluencer.h"

namespace stone {

class DeclState;

enum class TypeStateKind {
  Builtin,
  Function,
};

class alignas(1 << TypeAlignInBits) TypeState : public Artifact {

  // The type that we are dealing with
  TypeKind kind = TypeKind::None;

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
  explicit TypeState(TypeKind kind) : kind(kind) {}

  virtual ArtifactKind GetTypeStateKind() const = 0;

public:
  void SetKind(TypeKind K) { kind = K; }
  TypeKind GetKind() { return kind; }

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

  ArtifactKind GetArtifactKind() const override {
    return ArtifactKind::TypeState;
  }
};

// May have to pass the Module
class BuiltinTypeState final : public TypeState {
public:
  BuiltinTypeState() {}
};

// TypeState
//  ├── BuiltinTypeState
//  ├── NominalTypeState
//  ├── SugTypeState
//  ├── AccessTypeState
//  ├── DeducedTypeState
//  └── FunctionTypeState

} // namespace stone

#endif