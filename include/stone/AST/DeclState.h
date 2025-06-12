#ifndef STONE_AST_DECLSTATE_H
#define STONE_AST_DECLSTATE_H

#include "stone/AST/ASTAllocation.h"
#include "stone/AST/DeclName.h"
#include "stone/AST/TypeAlignment.h"

#include "llvm/ADT/BitVector.h"
#include "llvm/ADT/DenseMap.h"
#include "llvm/Support/Allocator.h"

#include <cassert>

namespace stone {

class alignas(1 << DeclAlignInBits) DeclState
    : public ASTAllocation<DeclState> {

  // The ASTSession associated with this DeclSate
  ASTSession &session;

  // The declaration name
  DeclName declName;

public:
  // Every DeclState must have a context
  explicit DeclState(ASTSession &session);

public:
  ASTSession &GetASTSession() { return session; }

  void SetDeclName(DeclName name) { declName = name; }
  DeclName GetDeclName() { return declName; }
};

} // namespace stone
#endif
