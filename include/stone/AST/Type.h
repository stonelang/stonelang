#ifndef STONE_AST_TYPE_H
#define STONE_AST_TYPE_H

#include "stone/AST/Alignments.h"
#include "stone/AST/InlineBitfield.h"
#include "stone/AST/Node.h"
#include "stone/AST/TypeKind.h"

#include "llvm/ADT/APFloat.h"
#include "llvm/ADT/APInt.h"
#include "llvm/ADT/APSInt.h"
#include "llvm/ADT/ArrayRef.h"
#include "llvm/ADT/FoldingSet.h"
#include "llvm/Support/Casting.h"
#include "llvm/Support/TrailingObjects.h"

namespace stone {

class Type;
class TypeFlight;

/// \brief The root base class for all semantic types in the Stone language.
///
/// Each concrete `Type` (e.g., `Int32Type`, `StructType`) inherits from this
/// base. The core type information is accessed through a lightweight handle,
/// `TypeFlight`, which carries all external state such as modifiers,
/// constraints, or inferred properties.
///
/// All `Type` subclasses are created through macros in `TypeNode.def`, and are
/// aligned to `TypeAlignInBits` to support efficient allocation and casting.
class alignas(1 << TypeAlignInBits) Type : public Node<NodeKind::Type, Type> {
  using Base = Node<NodeKind::Type, Type>;
  TypeFlight *flight = nullptr;

public:
  using Base::Base;
  /// \brief Constructs a type node with the associated metadata wrapper.
  explicit Type(TypeFlight *flight);

  /// \returns The specific kind of this type (e.g., Int32, Struct, Auto).
  TypeKind GetKind() const;

  /// \returns The associated `TypeFlight`, which stores additional metadata.
  TypeFlight *GetFlight() { return flight; }

  /// \returns True if this type is canonical (i.e., resolved, not sugar).
  bool IsCanType() const;

  /// \brief RTTI support: check whether a given Type pointer is valid.
  static bool classof(const Type *ty);

  /// \brief RTTI support: check whether a generic Node is a Type.
  static bool classof(const Node *node);
};

/// \brief Macro to generate concrete type subclasses (e.g., `IntType`,
/// `StructType`).
///
/// This macro is expanded for every entry in `TypeNode.def`. Each subclass
/// wraps a `TypeFlight*` and provides LLVM-style `classof` support.
///
/// Example:
/// \code
/// class Int32Type final : public Type {
/// public:
///   explicit Int32Type(TypeFlight *flight) : Type(flight) {}
///   static bool classof(const Type *T);
/// };
/// \endcode
#define TYPE(ID, Parent)                                                       \
  class ID##Type final : public Parent {                                       \
  public:                                                                      \
    explicit ID##Type(TypeFlight *flight) : Parent(flight) {}                  \
    static bool classof(const Type *T);                                        \
  };

#include "stone/AST/TypeNode.def"

} // namespace stone

#endif // STONE_AST_TYPE_H
