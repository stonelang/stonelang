#ifndef STONE_AST_DECL_H
#define STONE_AST_DECL_H

#include "stone/AST/AST.h"
#include "stone/AST/Identifier.h"
#include "stone/AST/InlineBitfield.h"
#include "stone/AST/TypeAlignment.h"
#include "llvm/Support/Casting.h"

#include <stdint.h>

namespace stone {

class DeclContext;
class ASTContext;
using DeclContextOrASTContext = llvm::PointerUnion<DeclContext *, ASTContext *>;

enum class DeclKind : uint8_t {
  None = 0,
#define DECL(Id, Parent) Id,
#define LAST_DECL(Id) Count = Id,
#define DECL_RANGE(Id, FirstId, LastId)                                        \
  First##Id##Decl = FirstId, Last##Id##Decl = LastId,
#include "stone/AST/DeclNode.def"
};

enum : uint8_t {
  NumDeclKindBits = stone::CountBitsUsed(static_cast<unsigned>(DeclKind::Count))
};

// Introduces a name and associates it with a type such as:
// int x where x is the declaration, int is the type.
class alignas(1 << DeclAlignInBits) Decl : public ASTAllocation<Decl> {
  DeclKind kind;

public:
  Decl();

public:
  DeclKind GetKind() const { return kind; }
  bool IsJoin() const { return kind == DeclKind::Join; }

public:
  static bool classof(const Decl *D) {
    return D->GetKind() >= DeclKind::FirstValueDecl &&
           D->GetKind() <= DeclKind::LastValueDecl;
  }
};

class ValueDecl : public Decl {
public:
};

class TypeDecl : public ValueDecl {
public:
};

class TemplateDecl {};

class UsingDecl : public ValueDecl {};

class JoinDecl : public ValueDecl {

public:
  JoinDecl();
};

class SpaceDecl : public ValueDecl {
public:
  SpaceDecl();
};

class TrustDecl final : public Decl {
public:
};

} // namespace stone
#endif
