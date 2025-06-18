#ifndef STONE_AST_DECLKIND_H
#define STONE_AST_DECLKIND_H

#include "stone/AST/InlineBitfield.h"

#include <cstdint>

namespace stone {
// DeclKind enum class definition
enum class DeclKind : uint8_t {
  None = 0,
// Core declaration macros
#define DECL(ID, Parent) ID,
// Ranges
#define LAST_DECL(ID) Count = ID,
#define DECL_RANGE(ID, FirstId, LastId) First##ID = FirstId, Last##ID = LastId,
#include "stone/AST/DeclNode.def"
};
enum : uint8_t {
  NumDeclKindBits = stone::CountBitsUsed(static_cast<unsigned>(DeclKind::Count))
};

// namespace stone {

// enum class DeclKind : uint8_t {
//   // Root declarations
//   Join,
//   Using,

// Builtin
//   // Concrete nominal types
//   Enum,
//   Struct,
//   Interface,

//   // Space declarations
//   Space,

//   // Alias type
//   Alias,

//   // Function declarations
//   Fun,
//   Constructor,
//   Destructor,

//   // Storage declarations
//   Var,
//   Param,

//   // Operator declarations
//   New,
//   Free,
//   InfixOperator,
//   PrefixOperator,
//   PostfixOperator,

//   // Conditional compilation
//   IfConfig,

//   // Always last
//   Count
// };

// // === Range Markers ===

// // Nominal types: Enum → Interface
// constexpr DeclKind FirstNominal = DeclKind::Enum;
// constexpr DeclKind LastNominal  = DeclKind::Interface;

// // Scope-like types: Nominal → Space
// constexpr DeclKind FirstScope   = DeclKind::Enum;
// constexpr DeclKind LastScope    = DeclKind::Space;

// // Function declarations: Fun → Destructor
// constexpr DeclKind FirstFunction = DeclKind::Fun;
// constexpr DeclKind LastFunction  = DeclKind::Destructor;

// // Storage declarations: Var → Param
// constexpr DeclKind FirstStorage = DeclKind::Var;
// constexpr DeclKind LastStorage  = DeclKind::Param;

// // Operator declarations: New → PostfixOperator
// constexpr DeclKind FirstOperator = DeclKind::New;
// constexpr DeclKind LastOperator  = DeclKind::PostfixOperator;

// } // namespace stone

} // namespace stone
#endif