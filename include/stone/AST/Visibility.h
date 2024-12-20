#ifndef STONE_AST_VSIBILITY_H
#define STONE_AST_VSIBILITY_H

#include "stone/AST/ASTAllocation.h"
#include "stone/AST/TypeAlignment.h"
#include "stone/Basic/LLVM.h"
#include "stone/Basic/STDAlias.h"
#include "stone/Basic/SrcLoc.h"

#include "llvm/ADT/PointerIntPair.h"
#include "llvm/ADT/SmallVector.h"

namespace stone {

enum class VisibilityLevel : uint8_t {
  None = 0,
  Private,
  Protected,
  Public,
  Global,
  File,
};
}
#endif 