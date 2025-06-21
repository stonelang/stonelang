#ifndef STONE_AST_TYPE_KIND_H
#define STONE_AST_TYPE_KIND_H

#include <cstdint>

namespace stone {

enum class TypeKind : uint8_t {
  None = 0,
#define TYPE(ID, Parent) ID,
#define LAST_TYPE(ID) Last_Type = ID,
#define TYPE_RANGE(Id, FirstId, LastId)                                        \
  First_##Id##Type = FirstId, Last_##Id##Type = LastId,
#include "stone/AST/TypeNode.def"
};

enum class IntKind : uint8_t {
  I8,
  I16,
  I32,
  I64,
  I128,
  ISize,
};

enum class UintKind : uint8_t {
  U8,
  U16,
  U32,
  U64,
  U128,
  USize,
};

enum class FloatKind : uint8_t {
  F16,
  F32,
  F64,
  F128,
};

} // namespace stone

#endif // STONE_AST_TYPE_KIND_H
