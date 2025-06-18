#ifndef STONE_AST_TYPE_KIND_H
#define STONE_AST_TYPE_KIND_H

#include <cstdint>

namespace stone {

enum class TypeKind : uint8_t {
  None = 0,
  // Root base type
  Object,

  // Builtin types
  Int,
  Int8,
  Int16,
  Int32,
  Int64,
  Int128,
  UInt,
  UInt8,
  UInt16,
  UInt32,
  UInt64,
  UInt128,
  Float,
  Float16,
  Float32,
  Float64,
  Float128,
  Bool,
  Char,
  Null,

  // Sugared types
  Alias,

  // Deduced
  Auto,

  // Function types
  Fun,

  // Access types - Pointers
  Ptr,
  Move,
  Own,
  Safe,

  // Access types - References
  Ref,

  // Nominal types
  Enum,
  Struct,
  Interface,

  // Always last
  Count
};

enum : uint8_t {
  NumTypeKindBits = stone::CountBitsUsed(static_cast<unsigned>(TypeKind::Count))
};

// === Range Markers ===

constexpr TypeKind FirstType = TypeKind::Object;

// Builtin
constexpr TypeKind FirstBuiltin = TypeKind::Int;
constexpr TypeKind LastBuiltin = TypeKind::Null;

// Sugared
constexpr TypeKind FirstSug = TypeKind::Alias;
constexpr TypeKind LastSug = TypeKind::Alias;

// Deduced
constexpr TypeKind FirstDeduced = TypeKind::Auto;
constexpr TypeKind LastDeduced = TypeKind::Auto;

// Function
constexpr TypeKind FirstFunction = TypeKind::Fun;
constexpr TypeKind LastFunction = TypeKind::Fun;

// Access: Pointer
constexpr TypeKind FirstPointer = TypeKind::Ptr;
constexpr TypeKind LastPointer = TypeKind::Safe;

// Access: Reference
constexpr TypeKind FirstReference = TypeKind::Ref;
constexpr TypeKind LastReference = TypeKind::Ref;

// Access (Pointer + Reference)
constexpr TypeKind FirstAccess = FirstPointer;
constexpr TypeKind LastAccess = LastReference;

// Nominal
constexpr TypeKind FirstNominal = TypeKind::Enum;
constexpr TypeKind LastNominal = TypeKind::Interface;

constexpr TypeKind LastType = TypeKind::Interface;

} // namespace stone

#endif // STONE_AST_TYPE_KIND_H
