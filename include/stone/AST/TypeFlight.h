#ifndef STONE_AST_TYPESTATE_H
#define STONE_AST_TYPESTATE_H

#include "stone/AST/Allocation.h"
#include "stone/AST/Number.h"
#include "stone/AST/Alignments.h"
#include "stone/AST/TypeInfluencer.h"
#include "stone/AST/TypeKind.h"

namespace stone {

class DeclFlight;

enum class TypeFlightKind {
  Builtin,
  Function,
  Pointer,
  Reference,
};

class alignas(1 << TypeAlignInBits) TypeFlight : public Allocation<TypeFlight> {
  // The type that owns this TypeFlight
  ///\ If the owner type is null
  Type *owner = nullptr;

  // The type that owns this TypeFlight
  ///\ If this is cannonical
  Type *canType = nullptr;

  // Source location of the type
  SrcLoc typeLoc;

  // The DeclFlight that owns this TypeFlight
  DeclFlight *declState = nullptr;

protected:
  // The type that we are dealing with
  TypeKind typeKind = TypeKind::None;

public:
  explicit TypeFlight(TypeKind typeKind) : typeKind(typeKind) {}

public:
  void SetTypeKind(TypeKind K) { typeKind = K; }
  TypeKind GetTypeKind() { return typeKind; }

  void SetCanType(Type *T) {
    assert(T && "TypeFlight cannot be assigned a null Type!");
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

  virtual TypeFlightKind GetTypeFlightKind() const = 0;
};

enum class BuiltinTypeTag : uint8_t {
  None = 0,
// Core declaration macros
#define BUILTIN_TYPE_TAG(T) T,
// Ranges
#define LAST_TAG(T) Count = ID,
#include "stone/AST/BuiltinTypeTag.def"
};

template <typename TagT, TagT TagValue>
class TagTypeFlight : public TypeFlight {
public:
  using TagType = TagT;
  static constexpr TagT tag = TagValue;
  TagTypeFlight(TypeKind kind) : TypeFlight(kind) {}
  TagT GetTag() const { return tag; }
  virtual TypeFlightKind GetTypeFlightKind() const = 0;
};

template <TypeKind Tag>
class SimpleTaggedTypeFlight : public TagTypeFlight<TypeKind, Tag> {
public:
  using Base = TagTypeFlight<TypeKind, Tag>;
  SimpleTaggedTypeFlight() : Base(Tag) {}

public:
};

template <TypeKind Tag>
class IntegerTaggedTypeFlight : public TagTypeFlight<TypeKind, Tag> {
public:
  using Base = TagTypeFlight<TypeKind, Tag>;
  IntegerTaggedTypeFlight() : Base(Tag) {}

public:
};

template <TypeKind Tag>
class FloatTaggedTypeFlight : public TagTypeFlight<TypeKind, Tag> {
public:
  using Base = TagTypeFlight<TypeKind, Tag>;
  FloatTaggedTypeFlight() : Base(Tag) {}

public:
};

template <BuiltinTypeTag TagValue>
class BuiltinTypeFlight : public TagTypeFlight<BuiltinTypeTag, TagValue> {
public:
  using Base = TagTypeFlight<BuiltinTypeTag, TagValue>;
  static constexpr BuiltinTypeTag tag = TagValue;

  BuiltinTypeFlight(TypeKind kind) : Base(kind) {}

public:
  TypeFlightKind GetTypeFlightKind() const override {
    return TypeFlightKind::Builtin;
  }
};

// TypeFlight *T = new BuiltinTypeFlight<BuiltinTypeTag::Int32>();
// if (T->GetKind() == TypeKind::Builtin) {
//   auto *BT = static_cast<BuiltinTypeFlight<BuiltinTypeTag::Int32> *>(T);
//   if (BT->GetBuiltinTag() == BuiltinTypeTag::Int32) {
//     // ...
//   }
// }
// template <typename T>
// concept IsBuiltinIntType = requires(T t) {
//   { T::GetTag() } -> std::same_as<BuiltinTypeTag>;
//   requires T::GetTag() == BuiltinTypeTag::Int32;
// };

// Factory example
// std::unique_ptr<Type> BuildInt32Type() {
//   auto state = new BuiltinTypeFlight<BuiltinTypeTag::Int32>(TypeKind::Int);
//   return std::make_unique<IntType>(state);
// }
// if (auto kind = T->GetFlight()->GetKind(); kind == TypeKind::Int) {
//   auto tag = cast<BuiltinTypeFlightBase>(T->GetFlight())->GetTag();
//   if (tag == BuiltinTypeTag::Int32) ...
// }

enum class FunTypeKind : uint8_t {
  Normal = 0,  // top-level or modular fun Main()
  Method,      // join A fun Do()
  Constructor, // join A fun A()
  Destructor,  // join A fun ~A()
  Defer,       // ~fun() {}
  Anonymous    // fun() {} — unbound
};

//===----------------------------------------------------------------------===//
// Base: FunTypeFlight
//===----------------------------------------------------------------------===//
class FunTypeFlight : public TypeFlight {
  FunTypeKind funTypeKind;

public:
  FunTypeFlight(FunTypeKind funTypeKind)
      : TypeFlight(TypeKind::Fun), funTypeKind(funTypeKind) {}
  FunTypeKind GetFunTypeKind() const { return funTypeKind; }

public:
  TypeFlightKind GetTypeFlightKind() const override {
    return TypeFlightKind::Function;
  }
  bool IsMethod() const { return funTypeKind == FunTypeKind::Method; }
  bool IsConstructor() const { return funTypeKind == FunTypeKind::Constructor; }
  bool IsDestructor() const { return funTypeKind == FunTypeKind::Destructor; }
  bool IsDefer() const { return funTypeKind == FunTypeKind::Defer; }
  bool IsAnonymous() const { return funTypeKind == FunTypeKind::Anonymous; }
  bool IsNormal() const { return funTypeKind == FunTypeKind::Normal; }
};

//===----------------------------------------------------------------------===//
// Specializations (optional, for static type dispatch)
//===----------------------------------------------------------------------===//

// class NormalFunTypeFlight : public FunTypeFlight {
// public:
//   NormalFunTypeFlight() : FunTypeFlight(FunTypeKind::Normal) {}
// };

// class MethodFunTypeFlight : public FunTypeFlight {
// public:
//   MethodFunTypeFlight() : FunTypeFlight(FunTypeKind::Method) {}
// };

// class ConstructorFunTypeFlight : public FunTypeFlight {
// public:
//   ConstructorFunTypeFlight() : FunTypeFlight(FunTypeKind::Constructor)
//   {}
// };

// class DestructorFunTypeFlight : public FunTypeFlight {
// public:
//   DestructorFunTypeFlight() : FunTypeFlight(FunTypeKind::Destructor) {}
// };

// class DeferFunTypeFlight : public FunTypeFlight {
// public:
//   DeferFunTypeFlight() : FunTypeFlight(FunTypeKind::Defer) {}
// };

// class AnonymousFunTypeFlight : public FunTypeFlight {
// public:
//   AnonymousFunTypeFlight() : FunTypeFlight(FunTypeKind::Anonymous) {}
// };

// TypeFlight
//  ├── BuiltinTypeFlight
//  ├── NominalTypeFlight
//  ├── SugTypeFlight
//  ├── AccessTypeFlight
//  ├── DeducedTypeFlight
//  └── FunTypeFlight
///     UniqueTypeFlight

class PtrTypeFlight : public TypeFlight {
public:
  PtrTypeFlight() : TypeFlight(TypeKind::Ptr) {}

public:
  TypeFlightKind GetTypeFlightKind() const override {
    return TypeFlightKind::Pointer;
  }
};

} // namespace stone

#endif