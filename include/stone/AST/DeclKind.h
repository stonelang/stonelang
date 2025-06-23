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
  DeclKindBitCount =
      stone::CountBitsUsed(static_cast<unsigned>(DeclKind::Count))
};

// inline bool IsTypeDecl(DeclKind K) {
//   return K >= DeclKind::TypeDecl && K <= DeclKind::LastTypeDecl;
// }

// inline bool IsOperatorDecl(DeclKind K) {
//   return K >= DeclKind::FirstOperator && K <= DeclKind::LastOperator;
// }

// inline bool IsBindingDecl(DeclKind K) {
//   return K >= DeclKind::FirstBinding && K <= DeclKind::LastBinding;
// }

// inline bool IsTopLevelDecl(DeclKind K) {
//   return K >= DeclKind::Join && K <= DeclKind::Macro;
// }

} // namespace stone
#endif