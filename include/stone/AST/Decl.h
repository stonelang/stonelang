#ifndef STONE_AST_DECL_H
#define STONE_AST_DECL_H

#include "stone/AST/DeclKind.h"
#include "stone/AST/Identifier.h"
#include "stone/AST/Tree.h"
#include "stone/AST/TypeAlignment.h"
#include "llvm/Support/Casting.h"

#include <stdint.h>

namespace stone {

class DeclState;
class FunDecl;
class JoinDecl;
class SpaceDecl;
class UsingDecl;

class alignas(1 << DeclAlignInBits) Decl : public Tree {
  DeclState *DS = nullptr;

public:
  Decl(DeclState *DS);

public:
  DeclState *GetDeclState() { return DS; }
  ArtifactKind GetArtifactKind() const override { return ArtifactKind::Decl; }

public:
  static bool classof(const Decl *D) {
    return D->GetDeclKind() >= DeclKind::Join &&
           D->GetDeclKind() < DeclKind::Count;
  }
  static bool classof(const Artifact *artifact) {
    return artifact->GetArtifactKind() == ArtifactKind::Decl;
  }
};

class ScopeDecl final : public Decl {
  llvm::SmallVector<Decl *, 8> members;

public:
  ScopeDecl(DeclState *DS) : Decl(DS) {}
};

class SpaceDecl final : public ScopeDecl {
public:
  SpaceDecl(DeclState *DS) : ScopeDecl(DS) {}
};

class JoinDecl final : public Decl {

public:
  JoinDecl(DeclState *DS) : Decl(DS) {}
};

class UsingDecl final : public Decl {

public:
  UsingDecl(DeclState *DS) : Decl(DS) {}
};

class MacroDecl : public Decl {
public:
  MacroDecl(DeclState *DS) : Decl(DS) {}
};

class TemplateDecl : public Decl {
public:
  TemplateDecl(DeclState *DS) : Decl(DS) {}
};

class TypeDecl : public TemplateDecl {
public:
  TypeDecl(DeclState *DS) : TemplateDecl(DS) {}
};

class BuiltinDecl : public TypeDecl {
public:
  BuiltinDecl(DeclState *DS) : TypeDecl(DS) {}
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
