#ifndef STONE_AST_TYPESTATE_H
#define STONE_AST_TYPESTATE_H

#include "stone/AST/Artifact.h"
#include "stone/AST/MemoryAllocation.h"
#include "stone/AST/Number.h"
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

/// Represents state for built-in types like int, float, char, bool, etc.
class BuiltinTypeState : public TypeState {
public:
  BuiltinTypeState(TypeKind kind) : TypeState(kind) {}

public:
  bool IsNumberType() const;
  NumberBitWidth GetNumberBitWidth() const;

  bool IsInt() const {
    return kind >= TypeKind::Int && kind <= TypeKind::UInt128;
  }
  bool IsSInt() const {
    return kind >= TypeKind::Int && kind <= TypeKind::Int128;
  }
  bool IsUInt() const {
    return kind >= TypeKind::UInt && kind <= TypeKind::UInt128;
  }
  bool IsFloat() const {
    return kind >= TypeKind::Float && kind <= TypeKind::Float128;
  }
  bool IsChar() const { return kind == TypeKind::Char; }
  bool IsBool() const { return kind == TypeKind::Bool; }

public:
  DeclStateKind GetDeclStateKind() const override {
    return DeclStateKind::Builtin;
  }

public:
  static bool IsNumberType(TypeKind kind) const;
  static NumberBitWidth GetNumberBitWidth(TypeKind kind) const;
};

class FunctionTypeState final : public TypeState {
public:
  FunctionTypeState(TypeKind kind);

public:
  DeclStateKind GetDeclStateKind() const override {
    return DeclStateKind::Function;
  }

public:
  static bool IsNumberType(TypeKind kind) const;
  static NumberBitWidth GetNumberBitWidth(TypeKind kind) const;
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