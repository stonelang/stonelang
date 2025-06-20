#ifndef STONE_AST_DECL_H
#define STONE_AST_DECL_H

#include "stone/AST/DeclKind.h"
#include "stone/AST/Identifier.h"
#include "stone/AST/Node.h"
#include "stone/AST/TypeAlignment.h"
#include "stone/Support/SrcLoc.h"
#include "llvm/Support/Casting.h"

#include <stdint.h>

namespace stone {
class Artifact;
class DeclState;

class alignas(1 << DeclAlignInBits) Decl : public Node {
  DeclState *DS = nullptr;

public:
  explicit Decl(DeclState *DS);
  DeclState *GetDeclState();

public:
  DeclKind GetKind() const;
  SrcLoc GetKindLoc() const;
  Identifier GetName() const;
  SrcLoc GetNameLoc() const;
  ArtifactKind GetArtifactKind() const override { return ArtifactKind::Decl; }

public:
  static bool classof(const Decl *D);
  static bool classof(const Artifact *artifact);
};

// === Top-Level Declarations ===
// class JoinDecl final : public Decl {
// public:
//   explicit JoinDecl(DeclState *DS) : Decl(DS) {}
// };

class SpaceDecl final : public Decl {
public:
  explicit SpaceDecl(DeclState *DS) : Decl(DS) {}
};

// class FunDecl final : public Decl {
// public:
//   explicit FunDecl(DeclState *DS) : Decl(DS) {}
// };

// DeclState(DeclState(DeclKind::Some))
// class SomeDecl final : public Decl {
// public:
//   explicit SomeDecl(DeclState *DS) : Decl(DS) {}
// };

// class JustDecl final : public Decl {
// public:
//   explicit JustDecl(DeclState *DS) : Decl(DS) {}
// };

// class IfConfigDecl final : public Decl {
// public:
//   explicit IfConfigDecl(DeclState *DS) : Decl(DS) {}
// };

// class MacroDecl final : public Decl {
// public:
//   explicit MacroDecl(DeclState *DS) : Decl(DS) {}
// };

// // === Type Declarations ===
// class TypeDecl : public Decl {
// public:
//   explicit TypeDecl(DeclState *DS) : Decl(DS) {}
// };

// class AliasDecl final : public TypeDecl {
// public:
//   explicit AliasDecl(DeclState *DS) : TypeDecl(DS) {}
// };

// class StructDecl final : public TypeDecl {
// public:
//   explicit StructDecl(DeclState *DS) : TypeDecl(DS) {}
// };

// class InterfaceDecl final : public TypeDecl {
// public:
//   explicit InterfaceDecl(DeclState *DS) : TypeDecl(DS) {}
// };

// class EnumDecl final : public TypeDecl {
// public:
//   explicit EnumDecl(DeclState *DS) : TypeDecl(DS) {}
// };

// class UsingDecl final : public TypeDecl {
// public:
//   explicit UsingDecl(DeclState *DS) : TypeDecl(DS) {}
// };

// // === Operator Declarations ===
// class OperatorDecl : public Decl {
// public:
//   explicit OperatorDecl(DeclState *DS) : Decl(DS) {}
// };

// class NewOperatorDecl final : public OperatorDecl {
// public:
//   explicit NewOperatorDecl(DeclState *DS) : OperatorDecl(DS) {}
// };

// class FreeOperatorDecl final : public OperatorDecl {
// public:
//   explicit FreeOperatorDecl(DeclState *DS) : OperatorDecl(DS) {}
// };

// class InfixOperatorDecl final : public OperatorDecl {
// public:
//   explicit InfixOperatorDecl(DeclState *DS) : OperatorDecl(DS) {}
// };

// class PrefixOperatorDecl final : public OperatorDecl {
// public:
//   explicit PrefixOperatorDecl(DeclState *DS) : OperatorDecl(DS) {}
// };

// class PostfixOperatorDecl final : public OperatorDecl {
// public:
//   explicit PostfixOperatorDecl(DeclState *DS) : OperatorDecl(DS) {}
// };

// // === Binding Declarations ===
// class BindingDecl : public Decl {
// public:
//   explicit BindingDecl(DeclState *DS) : Decl(DS) {}
// };

// class VarDecl : public BindingDecl {
// public:
//   explicit VarDecl(DeclState *DS) : BindingDecl(DS) {}
// };

// class ParamDecl final : public VarDecl {
// public:
//   explicit ParamDecl(DeclState *DS) : VarDecl(DS) {}
// };

// #define DECL(ID, Parent)                     \
// class ID##Decl : public Parent {             \
// public:                                      \
//   ID##Decl(DeclState* DS) : Parent(DS) {}    \
// };

// #define ABSTRACT_DECL(ID, Parent) \
// class ID##Decl : public Parent {  \
// public:                           \
//   ID##Decl(DeclState* DS) : Parent(DS) {} \
// };

} // namespace stone

#endif
