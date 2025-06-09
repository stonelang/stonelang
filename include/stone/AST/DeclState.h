#ifndef STONE_AST_DECLSTATE_H
#define STONE_AST_DECLSTATE_H

#include "stone/AST/AST.h"
#include "stone/AST/TypeAlignment.h"

#include "llvm/ADT/BitVector.h"
#include "llvm/ADT/DenseMap.h"
#include "llvm/Support/Allocator.h"

#include <cassert>

namespace stone {

class alignas(1 << DeclAlignInBits) DeclState
    : public ASTAllocation<DeclState> {

  // The ASTContext associated with this DeclSate
  ASTContext &astContext;

public:
  // Every DeclState must have a context
  explicit DeclState(ASTContext &astContext);
};

} // namespace stone
#endif
