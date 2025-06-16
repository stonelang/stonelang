#ifndef STONE_AST_DECLSTATE_H
#define STONE_AST_DECLSTATE_H

#include "stone/AST/ASTAllocation.h"
#include "stone/AST/DeclInfluencer.h"
#include "stone/AST/DeclKind.h"
#include "stone/AST/DeclName.h"
#include "stone/AST/Generics.h"
#include "stone/AST/TypeAlignment.h"
#include "stone/AST/TypeInfluencer.h"
#include "stone/AST/TypeState.h"

#include "llvm/ADT/BitVector.h"
#include "llvm/ADT/DenseMap.h"
#include "llvm/Support/Allocator.h"

#include <cassert>

namespace stone {
class Decl;
class DeclState;
class TypeState;
class ASTSession;

class alignas(1 << DeclAlignInBits) DeclState final
    : public ASTAllocation<DeclState> {
  friend Decl;

  // The ASTSession associated with this DeclSate
  ASTSession &session;

  // The declaration kind
  DeclKind kind;

  // Source location for the decl kind
  SrcLoc kindLoc;

  // The declaration name
  DeclName declName;

  // Source location for the decl name.
  SrcLoc declNameLoc;

  // The Decl associated with this DeclState
  Decl *owner = nullptr;

  // The parent decl state
  DeclState *parent = nullptr;

  // SpaceDecl* space = nullptr;

  // The properties for the DeclState
  DeclInfluencerList declInfluencerList;

  // The properties for the DeclState
  TypeInfluencerList typeInfluencerList;

  GenericParamList *genericParamList;

public:
  // Direct comparison is disabled for states
  void operator==(DeclState D) const = delete;
  void operator!=(DeclState D) const = delete;

  // Every DeclState must have a context
  explicit DeclState(ASTSession &session);

public:
  ASTSession &GetASTSession() { return session; }

  void SetKind(DeclKind K) { kind = K; }
  DeclKind GetKind() { return kind; }

  SrcLoc GetKindLoc() { return declNameLoc; }
  void SetKindLoc(SrcLoc currentLoc) { declNameLoc = currentLoc; }

  void SetName(DeclName name) { declName = name; }
  DeclName GetName() { return declName; }

  SrcLoc GetNameLoc() { return declNameLoc; }
  void SetNameLoc(SrcLoc currentLoc) { declNameLoc = currentLoc; }

  void SetOwner(Decl *D) { owner = D; }
  Decl *GetOwner() { return owner; }
  bool HasOwner() { return owner != nullptr; }

  void SetParent(DeclState *P) { parent = P; }
  DeclState *GetParent() { return parent; }

  TypeInfluencerList &GetTypeInfluencerList() { return typeInfluencerList; }
  DeclInfluencerList &GetDeclInfluencerList() { return declInfluencerList; }

  void SetGenericParamList(GenericParamList *GPL) { genericParamList = GPL; }
  GenericParamList GetGenericParamList() { return genericParamList; }

public:
  bool IsJoin() { return GetKind() == DeclKind::Join; }
  bool IsSpace() { return GetKind() == DeclKind::Space; }
  bool IsUsing() { return GetKind() == DeclKind::Using; }
};

class GenericDeclState : public DeclState {
public:
  // Every DeclState must have a context
  explicit GenericDeclState(ASTSession &session);
};

} // namespace stone
#endif
