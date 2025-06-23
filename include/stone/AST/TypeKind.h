#ifndef STONE_AST_TYPE_KIND_H
#define STONE_AST_TYPE_KIND_H

#include "stone/AST/InlineBitfield.h"

#include <cstdint>

namespace stone {
//===----------------------------------------------------------------------===//
// Bit Size Constants for Primitive Types
// These represent the number of bits used to store fundamental integer values.
// They are used throughout the type system, IR generation, and ABI layout.
// Avoid hardcoded literals like `8` or `32` in logic — use these named
// constants for readability, semantic clarity, and future portability.
//===----------------------------------------------------------------------===//

// 8-bit integers (e.g.,  int8,  uint8)
constexpr unsigned BitSize8 = 8;

// 16-bit integers (e.g., int16, uint16)
constexpr unsigned BitSize16 = 16;

// 32-bit integers (e.g., int32, uint32, float32)
constexpr unsigned BitSize32 = 32;

// 64-bit integers (e.g., int64, uint64, float64)
constexpr unsigned BitSize64 = 64;

// 128-bit integers (e.g., int128, uint128, float128)
constexpr unsigned BitSize128 = 128;

enum class TypeKind : uint8_t {
  None = 0,
#define TYPE(ID, Parent) ID,
#define LAST_TYPE(ID) Count = ID,
#define TYPE_RANGE(ID, FirstId, LastId) First##ID = FirstId, Last##ID = LastId,
#include "stone/AST/TypeNode.def"
};

enum : uint8_t {
  TypeKindBitCount =
      stone::CountBitsUsed(static_cast<unsigned>(TypeKind::Count))
};

inline bool IsNominalType(TypeKind K) {
  return K >= TypeKind::FirstNominal && K <= TypeKind::LastNominal;
}

inline bool IsMagicType(TypeKind K) {
  return K >= TypeKind::FirstMagic && K <= TypeKind::LastMagic;
}

inline bool IsAccessType(TypeKind K) {
  return K >= TypeKind::FirstAccess && K <= TypeKind::LastAccess;
}

inline bool IsAggregateType(TypeKind K) {
  return K >= TypeKind::FirstAggregate && K <= TypeKind::LastAggregate;
}

} // namespace stone
#endif