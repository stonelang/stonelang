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
  DeclState *state = nullptr;

public:
  explicit Decl(DeclState *state);
  DeclState *GetState();
  template <typename T> T *GetStateAs() { return llvm::cast<T>(state); }

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

// // === Top-Level Declarations ===
// class JoinDecl final : public Decl {
// public:
//   explicit JoinDecl(DeclState *state) : Decl(state) {}
// };

// class SpaceDecl final : public Decl {
// public:
//   explicit SpaceDecl(DeclState *state) : Decl(state) {}
// };

// class FunDecl final : public Decl {
// public:
//   explicit FunDecl(DeclState *state) : Decl(state) {}
// };

// DeclState(DeclState(DeclKind::Some))
// class SomeDecl final : public Decl {
// public:
//   explicit SomeDecl(DeclState *state) : Decl(state) {}
// };

// class JustDecl final : public Decl {
// public:
//   explicit JustDecl(DeclState *state) : Decl(state) {}
// };

// class IfConfigDecl final : public Decl {
// public:
//   explicit IfConfigDecl(DeclState *state) : Decl(state) {}
// };

// class MacroDecl final : public Decl {
// public:
//   explicit MacroDecl(DeclState *state) : Decl(state) {}
// };

// // === Type Declarations ===
// class TypeDecl : public Decl {
// public:
//   explicit TypeDecl(DeclState *state) : Decl(state) {}
// };

// class AliasDecl final : public TypeDecl {
// public:
//   explicit AliasDecl(DeclState *state) : TypeDecl(state) {}
// };

// class StructDecl final : public TypeDecl {
// public:
//   explicit StructDecl(DeclState *state) : TypeDecl(state) {}
// };

// class InterfaceDecl final : public TypeDecl {
// public:
//   explicit InterfaceDecl(DeclState *state) : TypeDecl(state) {}
// };

// class EnumDecl final : public TypeDecl {
// public:
//   explicit EnumDecl(DeclState *state) : TypeDecl(state) {}
// };

// class UsingDecl final : public TypeDecl {
// public:
//   explicit UsingDecl(DeclState *state) : TypeDecl(state) {}
// };

// // === Operator Declarations ===
// class OperatorDecl : public Decl {
// public:
//   explicit OperatorDecl(DeclState *state) : Decl(state) {}
// };

// class NewOperatorDecl final : public OperatorDecl {
// public:
//   explicit NewOperatorDecl(DeclState *state) : OperatorDecl(state) {}
// };

// class FreeOperatorDecl final : public OperatorDecl {
// public:
//   explicit FreeOperatorDecl(DeclState *state) : OperatorDecl(state) {}
// };

// class InfixOperatorDecl final : public OperatorDecl {
// public:
//   explicit InfixOperatorDecl(DeclState *state) : OperatorDecl(state) {}
// };

// class PrefixOperatorDecl final : public OperatorDecl {
// public:
//   explicit PrefixOperatorDecl(DeclState *state) : OperatorDecl(state) {}
// };

// class PostfixOperatorDecl final : public OperatorDecl {
// public:
//   explicit PostfixOperatorDecl(DeclState *state) : OperatorDecl(state) {}
// };

// // === Binding Declarations ===
// class BindingDecl : public Decl {
// public:
//   explicit BindingDecl(DeclState *state) : Decl(state) {}
// };

// class VarDecl : public BindingDecl {
// public:
//   explicit VarDecl(DeclState *state) : BindingDecl(state) {}
// };

// class ParamDecl final : public VarDecl {
// public:
//   explicit ParamDecl(DeclState *state) : VarDecl(state) {}
// };

// #define ABSTRACT_DECL(ID, Parent) \
// class ID##Decl : public Parent {  \
// public:                           \
//   ID##Decl(DeclState* state) : Parent(state) {} \
// };

} // namespace stone

#endif
