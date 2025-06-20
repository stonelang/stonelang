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

// === Range Markers ===

// constexpr TypeKind FirstType = TypeKind::Object;

// // Builtin
// constexpr TypeKind FirstBuiltin = TypeKind::Int;
// constexpr TypeKind LastBuiltin = TypeKind::Null;

// // Sugared
// constexpr TypeKind FirstSug = TypeKind::Alias;
// constexpr TypeKind LastSug = TypeKind::Alias;

// // Deduced
// constexpr TypeKind FirstDeduced = TypeKind::Auto;
// constexpr TypeKind LastDeduced = TypeKind::Auto;

// // Function
// constexpr TypeKind FirstFunction = TypeKind::Fun;
// constexpr TypeKind LastFunction = TypeKind::Fun;

// // Access: Pointer
// constexpr TypeKind FirstPointer = TypeKind::Ptr;
// constexpr TypeKind LastPointer = TypeKind::Safe;

// // Access: Reference
// constexpr TypeKind FirstReference = TypeKind::Ref;
// constexpr TypeKind LastReference = TypeKind::Ref;

// // Access (Pointer + Reference)
// constexpr TypeKind FirstAccess = FirstPointer;
// constexpr TypeKind LastAccess = LastReference;

// // Nominal
// constexpr TypeKind FirstNominal = TypeKind::Enum;
// constexpr TypeKind LastNominal = TypeKind::Interface;

// constexpr TypeKind LastType = TypeKind::Interface;

} // namespace stone

#endif // STONE_AST_TYPE_KIND_H
