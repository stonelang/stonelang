#ifndef STONE_AST_VSIBILITY_H
#define STONE_AST_VSIBILITY_H

#include "stone/AST/TypeAlignment.h"
#include "stone/Support/LLVM.h"
#include "stone/Support/SrcLoc.h"

#include "llvm/ADT/PointerIntPair.h"
#include "llvm/ADT/SmallVector.h"

#include <stdint.h>

namespace stone {
class Decl;
class Type;

enum class VisibilityLevel : uint8_t {
  Local = 0, // Scope visibilty.
  Public     // Visible to other modules
};

/// At a high level, this checks the given declaration's signature does not
/// reference any other declarations that are less visible than the
/// declaration itself. Related checks may also be performed.
void CheckVisibility(Decl *D);

/// Check the Type visibility level
void CheckVisibility(Type *ty);

} // namespace stone
#endif