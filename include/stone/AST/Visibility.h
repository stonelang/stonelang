#ifndef STONE_AST_VSIBILITY_H
#define STONE_AST_VSIBILITY_H

#include "stone/AST/TypeAlignment.h"
#include "stone/Lang/LLVM.h"
#include "stone/Lang/SrcLoc.h"

#include "llvm/ADT/PointerIntPair.h"
#include "llvm/ADT/SmallVector.h"

#include <stdint.h>

namespace stone {
class Decl;
class Type;

enum class VisibilityLevel : uint8_t {
  Private = 0, // Only within the type or file
  Internal,    // Anywhere in the module
  Public       // Visible to other modules
};

/// At a high level, this checks the given declaration's signature does not
/// reference any other declarations that are less visible than the
/// declaration itself. Related checks may also be performed.
void CheckVisibility(Decl *D);

/// Check the Type visibility level
void CheckVisibility(Type *ty);

} // namespace stone
#endif