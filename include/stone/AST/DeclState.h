#ifndef STONE_AST_DECLSTATE_H
#define STONE_AST_DECLSTATE_H

#include "stone/AST/ASTAllocation.h"
#include "stone/AST/DeclInfluencer.h"
#include "stone/AST/DeclName.h"
#include "stone/AST/TypeAlignment.h"
#include "stone/AST/TypeInfluencer.h"
#include "stone/AST/TypeState.h"

#include "llvm/ADT/BitVector.h"
#include "llvm/ADT/DenseMap.h"
#include "llvm/Support/Allocator.h"

#include <cassert>

namespace stone {

class Decl;
class TypeState;
class ASTSession;

class alignas(1 << DeclAlignInBits) DeclState final
    : public ASTAllocation<DeclState> {

  // The ASTSession associated with this DeclSate
  ASTSession &session;

  // The declaration name
  DeclName declName;

  // The properties for the DeclState
  DeclInfluencerList declInfluencerList;

  // The properties for the DeclState
  TypeInfluencerList typeInfluencerList;

public:
  // Direct comparison is disabled for states
  void operator==(DeclState D) const = delete;
  void operator!=(DeclState D) const = delete;

  // Every DeclState must have a context
  explicit DeclState(ASTSession &session);

public:
  ASTSession &GetASTSession() { return session; }
  void SetDeclName(DeclName name) { declName = name; }
  DeclName GetDeclName() { return declName; }
  TypeInfluencerList &GetTypeInfluencerList() { return typeInfluencerList; }
  DeclInfluencerList &GetDeclInfluencerList() { return declInfluencerList; }
};

} // namespace stone
#endif
