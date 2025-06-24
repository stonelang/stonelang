#ifndef STONE_AST_TYPEFLIGHT_H
#define STONE_AST_TYPEFLIGHT_H

#include "stone/AST/Alignments.h"
#include "stone/AST/Allocation.h"
#include "stone/AST/Number.h"
#include "stone/AST/TypeInfluencer.h"
#include "stone/AST/TypeKind.h"

namespace stone {

class DeclFlight;

/// \brief Enumerates the different shape-kinds a `TypeFlight` may represent.
/// Used for dynamic dispatch without relying on RTTI.
enum class TypeFlightKind {
  Builtin,
  Function,
  Pointer,
  Reference,
};

/// \brief Base class for all `TypeFlight` specializations.
///
/// Provides a uniform interface for all type construction metadata.
/// This class encapsulates shared state such as parent linkage for nested types
/// (e.g., function return types, tuple elements), and enforces allocation
/// discipline through `Allocation<T>`.
///
/// \note This class must never be instantiated directly.
///       Only concrete `TypeFlight<K>` specializations should be used.
class alignas(1 << TypeAlignInBits) TypeFlightBase
    : public Allocation<TypeFlightBase> {
protected:
  TypeKind kind = TypeKind::None;

  /// \brief The parent `TypeFlight`, if this type is nested inside another.
  ///
  /// For example, in a function type `(Int32) -> Float64`, the return type
  /// `Float64` may reference the function’s `TypeFlight` as its parent.
  TypeFlightBase *parent = nullptr;

  /// \brief Protected constructor to prevent direct instantiation.
  explicit TypeFlightBase(TypeKind kind, TypeFlightBase *parent = nullptr)
      : kind(kind), parent(parent) {
    static_assert(
        !std::is_same_v<std::remove_cv_t<decltype(*this)>, TypeFlightBase>,
        "TypeFlightBase must never be instantiated directly.");
  }

public:
  TypeFlightBase(const TypeFlightBase &) = delete;
  TypeFlightBase &operator=(const TypeFlightBase &) = delete;

  /// \brief Sets the parent type flight.
  void SetParent(TypeFlightBase *P) { parent = P; }

  /// \returns The parent `TypeFlightBase`, if any.
  TypeFlightBase *GetParent() const { return parent; }

  /// \returns The static kind of this flight.
  TypeKind GetTypeKind() const { return kind; }
};

/// \brief A type-specific metadata container used during type resolution.
///
/// This class captures the *flight* (or construction-time state) for a type
/// and encodes its kind, relationships, and source provenance.
///
/// \tparam K The static tag identifying the concrete `TypeKind` of this flight.
template <TypeKind K> class TypeFlight : public TypeFlightBase {

  /// \brief The type instance that owns this `TypeFlight`.
  /// May be null during construction.
  Type *owner = nullptr;

  /// \brief Canonical representative of this type (used for equivalence).
  Type *canType = nullptr;

  /// \brief The source location where this type appeared.
  SrcLoc typeLoc;

  /// \brief Optional owner declaration (e.g., for named types in decls).
  DeclFlight *declState = nullptr;

public:
  /// \brief The static tag associated with this `TypeFlight`.
  static constexpr TypeKind kind = K;

  /// \brief Construct a `TypeFlight` with an optional parent.
  explicit TypeFlight(TypeFlightBase *parent = nullptr)
      : TypeFlightBase(K, parent) {}

  void SetCanType(Type *T) {
    assert(T && "TypeFlight cannot be assigned a null canonical Type!");
    canType = T;
  }
  Type *GetCanType() { return canType; }
  bool HasCanType() const { return canType != nullptr; }

  void SetOwner(Type *ty) { owner = ty; }
  Type *GetOwner() { return owner; }
  bool HasOwner() const { return owner != nullptr; }

  void SetLoc(SrcLoc loc) { typeLoc = loc; }
  SrcLoc GetLoc() const { return typeLoc; }
  bool HasLoc() const { return typeLoc.isValid(); }

  void SetDeclState(DeclFlight *d) { declState = d; }
  DeclFlight *GetDeclState() const { return declState; }
  bool HasDeclState() const { return declState != nullptr; }

  static bool classof(const TypeFlightBase *B) {
    return B && static_cast<const TypeFlight<K> *>(B)->kind == K;
  }

  static constexpr TypeKind GetKindStatic() { return K; }

  /// \returns The runtime flight-kind discriminator.
  virtual TypeFlightKind GetTypeFlightKind() const = 0;
};

// template <typename TagT, TagT TagValue>
// class TagTypeFlight : public TypeFlight {
// public:
//   using TagType = TagT;
//   static constexpr TagT tag = TagValue;
//   TagTypeFlight(TypeKind kind) : TypeFlight(kind) {}
//   TagT GetTag() const { return tag; }
//   virtual TypeFlightKind GetTypeFlightKind() const = 0;
// };

// template <TypeKind Tag>
// class SimpleTaggedTypeFlight : public TagTypeFlight<TypeKind, Tag> {
// public:
//   using Base = TagTypeFlight<TypeKind, Tag>;
//   SimpleTaggedTypeFlight() : Base(Tag) {}

// public:
// };

// template <TypeKind Tag>
// class IntegerTaggedTypeFlight : public TagTypeFlight<TypeKind, Tag> {
// public:
//   using Base = TagTypeFlight<TypeKind, Tag>;
//   IntegerTaggedTypeFlight() : Base(Tag) {}

// public:
// };

// template <TypeKind Tag>
// class FloatTaggedTypeFlight : public TagTypeFlight<TypeKind, Tag> {
// public:
//   using Base = TagTypeFlight<TypeKind, Tag>;
//   FloatTaggedTypeFlight() : Base(Tag) {}

// public:
// };

// template <BuiltinTypeTag TagValue>
// class BuiltinTypeFlight : public TagTypeFlight<BuiltinTypeTag, TagValue> {
// public:
//   using Base = TagTypeFlight<BuiltinTypeTag, TagValue>;
//   static constexpr BuiltinTypeTag tag = TagValue;

//   BuiltinTypeFlight(TypeKind kind) : Base(kind) {}

// public:
//   TypeFlightKind GetTypeFlightKind() const override {
//     return TypeFlightKind::Builtin;
//   }
// };

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

// enum class FunTypeKind : uint8_t {
//   Normal = 0,  // top-level or modular fun Main()
//   Method,      // join A fun Do()
//   Constructor, // join A fun A()
//   Destructor,  // join A fun ~A()
//   Defer,       // ~fun() {}
//   Anonymous    // fun() {} — unbound
// };

//===----------------------------------------------------------------------===//
// Base: FunTypeFlight
//===----------------------------------------------------------------------===//
// class FunTypeFlight : public TypeFlight {
//   FunTypeKind funTypeKind;

// public:
//   FunTypeFlight(FunTypeKind funTypeKind)
//       : TypeFlight(TypeKind::Fun), funTypeKind(funTypeKind) {}
//   FunTypeKind GetFunTypeKind() const { return funTypeKind; }

// public:
//   TypeFlightKind GetTypeFlightKind() const override {
//     return TypeFlightKind::Function;
//   }
//   bool IsMethod() const { return funTypeKind == FunTypeKind::Method; }
//   bool IsConstructor() const { return funTypeKind ==
//   FunTypeKind::Constructor; } bool IsDestructor() const { return funTypeKind
//   == FunTypeKind::Destructor; } bool IsDefer() const { return funTypeKind ==
//   FunTypeKind::Defer; } bool IsAnonymous() const { return funTypeKind ==
//   FunTypeKind::Anonymous; } bool IsNormal() const { return funTypeKind ==
//   FunTypeKind::Normal; }
// };

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

// class PtrTypeFlight : public TypeFlight {
// public:
//   PtrTypeFlight() : TypeFlight(TypeKind::Ptr) {}

// public:
//   TypeFlightKind GetTypeFlightKind() const override {
//     return TypeFlightKind::Pointer;
//   }
// };

} // namespace stone

#endif