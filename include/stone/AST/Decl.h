#ifndef STONE_AST_DECL_H
#define STONE_AST_DECL_H

#include "stone/AST/ASTUnit.h"
#include "stone/AST/Identifier.h"
#include "stone/AST/InlineBitfield.h"
#include "stone/AST/TypeAlignment.h"
#include "llvm/Support/Casting.h"

#include <stdint.h>

namespace stone {

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
class alignas(1 << DeclAlignInBits) Decl : public ASTUnit {
  DeclKind kind;

public:
  Decl(DeclKind kind, ASTSession &session);

public:
  DeclKind GetKind() const { return kind; }
  ASTUnitKind GetUnitKind() const override { return ASTUnitKind::Decl; }

public:
  bool IsJoin() const { return kind == DeclKind::Join; }

public:
  static bool classof(const Decl *D) {
    return D->GetKind() >= DeclKind::FirstValueDecl &&
           D->GetKind() <= DeclKind::LastValueDecl;
  }
  static bool classof(const ASTUnit *unit) {
    return unit->GetUnitKind() == ASTUnitKind::Decl;
  }
};

class ValueDecl : public Decl {
public:
  ValueDecl(DeclKind kind, ASTSession &session) : Decl(kind, session) {}
};

class FunctionDecl : public ValueDecl {
public:
  FunctionDecl(DeclKind kind, ASTSession &session) : ValueDecl(kind, session) {}
};

class FunDecl : public FunctionDecl {

public:
  FunDecl(ASTSession &session) : FunctionDecl(DeclKind::Fun, session) {}
};

class TypeDecl : public ValueDecl {
public:
  TypeDecl(DeclKind kind, ASTSession &session) : ValueDecl(kind, session) {}
};

// class TemplateDecl {};

// class UsingDecl : public ValueDecl {};

class JoinDecl : public ValueDecl {

public:
  JoinDecl();
};

// class SpaceDecl : public ValueDecl {
// public:
//   SpaceDecl();
// };

// class TrustDecl final : public Decl {
// public:
// };

} // namespace stone
#endif
