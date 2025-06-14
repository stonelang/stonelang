#ifndef STONE_AST_DECL_H
#define STONE_AST_DECL_H

#include "stone/AST/ASTUnit.h"
#include "stone/AST/Identifier.h"
#include "stone/AST/InlineBitfield.h"
#include "stone/AST/TypeAlignment.h"
#include "llvm/Support/Casting.h"

#include <stdint.h>

namespace stone {

class DeclState;

// DeclKind enum class definition
enum class DeclKind : uint8_t {
  None = 0,
  // Core declaration macros
#define DECL(ID, Parent) ID,
// Ranges
#define LAST_DECL(ID) Count = ID,
#define DECL_RANGE(ID, FirstId, LastId) \
  First##ID = FirstId, Last##ID = LastId,

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
  // void Evaluate(DeclActionKind kind);

public:
  static bool classof(const Decl *D) {
    return D->GetKind() >= DeclKind::FirstDecl &&
           D->GetKind() <= DeclKind::LastDecl;
  }
  static bool classof(const ASTUnit *unit) {
    return unit->GetUnitKind() == ASTUnitKind::Decl;
  }
};

enum class JoinDeclKind : uint8_t {
  Module = 0,
  Struct,
  Interface,
  Enum,
};

class JoinDecl : public Decl {
  JoinDeclKind kind;

public:
  JoinDecl(JoinDeclKind kind);

public:
  JoinDeclKind GetJoinDeclKind() { return kind; }

  bool IsModule() const { return kind == JoinDeclKind::Module; }
  bool IsStruct() const { return kind == JoinDeclKind::Struct; }
  bool IsInterface() const { return kind == JoinDeclKind::Interface; }
  bool IsEnum() const { return kind == JoinDeclKind::Enum; }
};

enum class UsingDeclKind : uint8_t {
  Module = 0,
  Struct,
  Interface,
  Enum,
  Fun,
  Macro,
};
class UsingDecl : public Decl {
  UsingDeclKind kind;

public:
  UsingDecl(UsingDeclKind kind);

public:
  UsingDeclKind GetUsingDeclKind() { return kind; }
};

class SpaceDecl : public Decl {
public:
  SpaceDecl();
};

class FunctionDecl : public Decl {
public:
  FunctionDecl(DeclKind kind, ASTSession &session) : Decl(kind, session) {}
};

class FunDecl : public FunctionDecl {

public:
  FunDecl(ASTSession &session) : FunctionDecl(DeclKind::Fun, session) {}
};

class TypeDecl : public Decl {
public:
  TypeDecl(DeclKind kind, ASTSession &session) : Decl(kind, session) {}
};

class AliasDecl : public TypeDecl {
public:
  AliasDecl(DeclKind kind, ASTSession &session) : TypeDecl(kind, session) {}
};

class StorageDecl : public Decl {
public:
  StorageDecl(DeclKind kind, ASTSession &session) : Decl(kind, session) {}
};

class VarDecl : public StorageDecl {

public:
  VarDecl(ASTSession &session) : StorageDecl(DeclKind::Var, session) {}
};

class TrustDecl final : public Decl {
public:
};

} // namespace stone
#endif
