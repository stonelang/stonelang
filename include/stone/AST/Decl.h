#ifndef STONE_AST_DECL_H
#define STONE_AST_DECL_H

#include "stone/AST/ASTUnit.h"
#include "stone/AST/DeclKind.h"
#include "stone/AST/Identifier.h"
#include "stone/AST/TypeAlignment.h"
#include "llvm/Support/Casting.h"

#include <stdint.h>

namespace stone {

class DeclState;

// Introduces a name and associates it with a type such as:
// int x where x is the declaration, int is the type.
class alignas(1 << DeclAlignInBits) Decl : public ASTUnit {
  DeclState *DS;

public:
  Decl(DeclState *DS);

public:
  DeclState *GetDeclState();
  DeclKind GetDeclKind() const;
  ASTUnitKind GetUnitKind() const override { return ASTUnitKind::Decl; }

  // void Evaluate(DeclActionKind kind);

public:
public:
  static bool classof(const Decl *D) {
    return D->GetDeclKind() >= DeclKind::FirstDecl &&
           D->GetDeclKind() <= DeclKind::LastDecl;
  }
  static bool classof(const ASTUnit *unit) {
    return unit->GetUnitKind() == ASTUnitKind::Decl;
  }
};

enum class JoinDeclKind : uint8_t {
  None = 0,
  Module,
  Struct,
  Interface,
  Enum,
};

class JoinDecl final : public Decl {
  JoinDeclKind joinKind;

public:
  JoinDecl(DeclState *DS) : Decl(DS) {}

public:
  void SetJoinKind(JoinDeclKind kind) { joinKind = kind; }
  JoinDeclKind GetJoinKind() { return joinKind; }

  bool IsModule() const { return joinKind == JoinDeclKind::Module; }
  bool IsStruct() const { return joinKind == JoinDeclKind::Struct; }
  bool IsInterface() const { return joinKind == JoinDeclKind::Interface; }
  bool IsEnum() const { return joinKind == JoinDeclKind::Enum; }
};

enum class UsingDeclKind : uint8_t {
  None = 0,
  Module,
  Struct,
  Interface,
  Enum,
  Fun,
  Macro,
};
class UsingDecl final : public Decl {
  UsingDeclKind usingKind;

public:
  UsingDecl(DeclState *DS) : Decl(DS) {}

public:
  void SetUsingKind(UsingDeclKind kind) { usingKind = kind; }
  UsingDeclKind GetUsingKind() { return usingKind; }
};

class SpaceDecl final : public Decl {
public:
  SpaceDecl(DeclState *DS) : Decl(DS) {}
};

class GenericDecl : public Decl {
public:
  GenericDecl(DeclState *DS) : Decl(DS) {}

public:
  bool HasSignature() const;
};

class TypeDecl : public GenericDecl {
public:
  TypeDecl(DeclState *DS) : GenericDecl(DS) {}
};

class AliasDecl final : public TypeDecl {
public:
  AliasDecl(DeclState *DS) : TypeDecl(DS) {}
};

class FunctionDecl : public GenericDecl {
public:
  FunctionDecl(DeclState *DS) : GenericDecl(DS) {}
};

class FunDecl : public FunctionDecl {

public:
  FunDecl(DeclState *DS) : FunctionDecl(DS) {}
};

class ConstructorDecl : public FunctionDecl {

public:
  ConstructorDecl(DeclState *DS) : FunctionDecl(DS) {}
};

class DestructorDecl : public FunctionDecl {

public:
  DestructorDecl(DeclState *DS) : FunctionDecl(DS) {}
};

class StorageDecl : public Decl {
public:
  StorageDecl(DeclState *DS) : Decl(DS) {}
};

class VarDecl : public StorageDecl {

public:
  VarDecl(DeclState *DS) : StorageDecl(DS) {}
};

class TrustDecl final : public Decl {
public:
  TrustDecl(DeclState *DS) : Decl(DS) {}
};

} // namespace stone
#endif
