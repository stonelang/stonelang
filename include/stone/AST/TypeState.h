#ifndef STONE_AST_TYPESTATE_H
#define STONE_AST_TYPESTATE_H

#include "stone/AST/Artifact.h"
#include "stone/AST/MemoryAllocation.h"
#include "stone/AST/Number.h"
#include "stone/AST/TypeAlignment.h"
#include "stone/AST/TypeInfluencer.h"
#include "stone/AST/TypeKind.h"

namespace stone {

class DeclState;

enum class TypeStateKind {
  Builtin,
  Function,
};

class alignas(1 << TypeAlignInBits) TypeState : public Artifact {
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

protected:
  // The type that we are dealing with
  TypeKind typeKind = TypeKind::None;

public:
  explicit TypeState(TypeKind typeKind) : typeKind(typeKind) {}

public:
  void SetTypeKind(TypeKind K) { typeKind = K; }
  TypeKind GetTypeKind() { return typeKind; }

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

  virtual TypeStateKind GetTypeStateKind() const = 0;
};

// May have to pass the Module

/// Represents state for built-in types like int, float, char, bool, etc.
class BuiltinTypeState : public TypeState {
public:
  BuiltinTypeState(TypeKind typeKind) : TypeState(typeKind) {}

public:
  // bool IsNumberType() const;
  // NumberBitWidth GetNumberBitWidth();

public:
  TypeStateKind GetTypeStateKind() const override {
    return TypeStateKind::Builtin;
  }

public:
  // static bool IsNumberType(TypeKind kind);
  // static NumberBitWidth GetNumberBitWidth(TypeKind kind);
};

enum class FunTypeStateKind : uint8_t {
  Normal = 0,  // top-level or modular fun Main()
  Method,      // join A fun Do()
  Constructor, // join A fun A()
  Destructor,  // join A fun ~A()
  Defer,       // ~fun() {}
  Anonymous    // fun() {} — unbound
};

//===----------------------------------------------------------------------===//
// Base: FunTypeState
//===----------------------------------------------------------------------===//
class FunTypeState : public TypeState {
  FunTypeStateKind funTypeStateKind;

public:
  FunTypeState(FunTypeStateKind funTypeStateKind)
      : TypeState(TypeKind::Fun), funTypeStateKind(funTypeStateKind) {}

public:
  TypeStateKind GetTypeStateKind() const override {
    return TypeStateKind::Function;
  }

  FunTypeStateKind GetFunTypeStateKind() const { return funTypeStateKind; }
  bool IsMethod() const { return funTypeStateKind == FunTypeStateKind::Method; }
  bool IsConstructor() const {
    return funTypeStateKind == FunTypeStateKind::Constructor;
  }
  bool IsDestructor() const {
    return funTypeStateKind == FunTypeStateKind::Destructor;
  }
  bool IsDefer() const { return funTypeStateKind == FunTypeStateKind::Defer; }
  bool IsAnonymous() const {
    return funTypeStateKind == FunTypeStateKind::Anonymous;
  }
  bool IsNormal() const { return funTypeStateKind == FunTypeStateKind::Normal; }
};

//===----------------------------------------------------------------------===//
// Specializations (optional, for static type dispatch)
//===----------------------------------------------------------------------===//

class NormalFunTypeState : public FunTypeState {
public:
  NormalFunTypeState() : FunTypeState(FunTypeStateKind::Normal) {}
};

class MethodFunTypeState : public FunTypeState {
public:
  MethodFunTypeState() : FunTypeState(FunTypeStateKind::Method) {}
};

class ConstructorFunTypeState : public FunTypeState {
public:
  ConstructorFunTypeState() : FunTypeState(FunTypeStateKind::Constructor) {}
};

class DestructorFunTypeState : public FunTypeState {
public:
  DestructorFunTypeState() : FunTypeState(FunTypeStateKind::Destructor) {}
};

class DeferFunTypeState : public FunTypeState {
public:
  DeferFunTypeState() : FunTypeState(FunTypeStateKind::Defer) {}
};

class AnonymousFunTypeState : public FunTypeState {
public:
  AnonymousFunTypeState() : FunTypeState(FunTypeStateKind::Anonymous) {}
};

// TypeState
//  ├── BuiltinTypeState
//  ├── NominalTypeState
//  ├── SugTypeState
//  ├── AccessTypeState
//  ├── DeducedTypeState
//  └── FunTypeState
///     UniqueTypeState

} // namespace stone

#endif