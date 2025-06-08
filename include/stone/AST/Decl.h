#ifndef STONE_AST_DECL_H
#define STONE_AST_DECL_H

#include <stdint.h>

#include "llvm/Support/Casting.h"

namespace stone {

enum class DeclKind : uint8_t {
  None,
#define DECL(Id, Parent) Id,
#define LAST_DECL(Id) Count = Id,
#define DECL_RANGE(Id, FirstId, LastId)                                        \
  First##Id##Decl = FirstId, Last##Id##Decl = LastId,
#include "stone/AST/DeclNode.def"
};

// enum : unsigned {
//   NumDeclKindBits =
//   stone::CountBitsUsed(static_cast<unsigned>(DeclKind::Count))
// };

} // namespace stone
#endif
