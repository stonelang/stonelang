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
class FunDecl;
class JoinDecl;
class SpaceDecl;
class UsingDecl;

// Introduces a name and associates it with a type such as:
// int x where x is the declaration, int is the type.
class alignas(1 << DeclAlignInBits) Decl : public ASTUnit {
  DeclState *DS;

public:
  Decl(DeclState *DS);

public:
  DeclState *GetDeclState();
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

class ScopeDecl final : public Decl {
  llvm::SmallVector<Decl *, 8> members;

public:
  ScopeDecl(DeclState *DS) : Decl(DS) {}

public:
  // llvm::ArrayRef<Decl*> GetMembers() const { return members; }
  // void AddMember(Decl* D) { members.push_back(D); }
};

class SpaceDecl final : public ScopeDecl {
public:
  SpaceDecl(DeclState *DS) : ScopeDecl(DS) {}
};

enum class JoinDeclKind : uint8_t {
  None = 0,
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
  bool IsStruct() const { return joinKind == JoinDeclKind::Struct; }
  bool IsInterface() const { return joinKind == JoinDeclKind::Interface; }
  bool IsEnum() const { return joinKind == JoinDeclKind::Enum; }
};

class DotPath {
  llvm::SmallVector<Identifier, 4> segments;

public:
  DotPath() = default;
  DotPath(llvm::ArrayRef<Identifier> segs)
      : segments(segs.begin(), segs.end()) {}

  llvm::ArrayRef<Identifier> GetSegments() const { return segments; }
  Identifier GetTail() const { return segments.back(); }
  Identifier GetHead() const { return segments.front(); }

  void Print(llvm::raw_ostream &os) const;
};

// using Physics.Quantum[struct Light];
// using Physics.Quantum[fun Fire];
// using Physics.Quantum[interface Particle];
enum class UsingDeclKind : uint8_t {
  None = 0,
  Module,
  Struct,
  Interface,
  Enum,
  Fun,
  Macro,
};
/// using A.B{C.D}
class UsingDecl final : public Decl {
  UsingDeclKind usingKind;

public:
  UsingDecl(DeclState *DS) : Decl(DS) {}

public:
  void SetUsingKind(UsingDeclKind kind) { usingKind = kind; }
  UsingDeclKind GetUsingKind() { return usingKind; }
};

class TemplateDecl : public Decl {
public:
  TemplateDecl(DeclState *DS) : Decl(DS) {}

public:
  // bool HasSignature() const;
};

class TypeDecl : public TemplateDecl {
public:
  TypeDecl(DeclState *DS) : TemplateDecl(DS) {}
};

class AliasDecl final : public TypeDecl {
public:
  AliasDecl(DeclState *DS) : TypeDecl(DS) {}
};

class FunctionDecl : public TemplateDecl {
public:
  FunctionDecl(DeclState *DS) : TemplateDecl(DS) {}
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

class OperatorDecl : public Decl {
public:
  OperatorDecl(DeclState *DS) : Decl(DS) {}
};

class NewOperatorDecl final : public OperatorDecl {
public:
  NewOperatorDecl(DeclState *DS) : OperatorDecl(DS) {}
};

class FreeOperatorDecl final : public OperatorDecl {
public:
  FreeOperatorDecl(DeclState *DS) : OperatorDecl(DS) {}
};

class PrefixOperatorDecl final : public OperatorDecl {
public:
  PrefixOperatorDecl(DeclState *DS) : OperatorDecl(DS) {}
};

class InfixOperatorDecl final : public OperatorDecl {
public:
  InfixOperatorDecl(DeclState *DS) : OperatorDecl(DS) {}
};
class PostfixOperatorDecl final : public OperatorDecl {
public:
  PostfixOperatorDecl(DeclState *DS) : OperatorDecl(DS) {}
};

} // namespace stone
#endif
