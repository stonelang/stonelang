#ifndef STONE_AST_DECLSTATE_H
#define STONE_AST_DECLSTATE_H

#include "stone/AST/Artifact.h"
#include "stone/AST/DeclInfluencer.h"
#include "stone/AST/DeclKind.h"
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
class DeclState;
class TypeState;
class ASTSession;
class ParamList;

// enum class DeclStateKind : uint8_t {
//   Parametric,
//   Template,
//   Using,
//   Join,
//   Builtin,
//   Function,
//   Alias
// };

enum class DeclStateKind : uint8_t {
  Builtin,
  Function,
};

class alignas(1 << DeclAlignInBits) DeclState final : public Artifact {
  // friend Decl;

  // The declaration kind
  DeclKind kind = DeclKind::None;

  // Source location for the decl kind
  SrcLoc kindLoc;

  // The declaration name
  DeclName declName;

  // Source location for the decl name.
  SrcLoc declNameLoc;

  // The Decl associated with this DeclState
  Decl *owner = nullptr;

  // The parent decl state -- maybe not
  DeclState *parent = nullptr;

  // The properties for the DeclState
  DeclInfluencerList declInfluencerList;

  // The properties for the DeclState
  TypeInfluencerList typeInfluencerList;

  ParamList *paramList;

public:
  // Direct comparison is disabled for states
  void operator==(DeclState D) const = delete;
  void operator!=(DeclState D) const = delete;

  // Every DeclState must have a context
  explicit DeclState(DeclState *parent = nullptr);

public:
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
  bool HasParent() { return parent != nullptr; }

  TypeInfluencerList &GetTypeInfluencerList() { return typeInfluencerList; }
  DeclInfluencerList &GetDeclInfluencerList() { return declInfluencerList; }

  void SetParamList(ParamList *PL) { paramList = PL; }
  ParamList GetParamList() { return paramList; }

  // template <typename T>
  // T *GetParamListAs() const {
  //   return llvm::dyn_cast<T>(params);
  // }

  //   oid SetParamList(std::unique_ptr<ParamList> p) {
  //   switch (p->GetKind()) {
  //     case ParamListKind::Any:
  //       assert(isa<TemplateDecl>(thisDecl) && "Only TemplateDecl can have
  //       AnyParamList"); break;
  //     case ParamListKind::Using:
  //       assert(isa<UsingDecl>(thisDecl) && "Only UsingDecl can have
  //       UsingParamList"); break;
  //     case ParamListKind::Space:
  //       assert(isa<SpaceDecl>(thisDecl) && "Only SpaceDecl can have
  //       SpaceParamList"); break;
  //   }

  //   paramList = std::move(p);
  // }

  ArtifactKind GetArtifactKind() const override {
    return ArtifactKind::DeclState;
  }
  virtual DeclStateKind GetDeclStateKind() const = 0;

public:
  bool IsJoin() { return GetKind() == DeclKind::Join; }
  bool IsSpace() { return GetKind() == DeclKind::Space; }
  bool IsUsing() { return GetKind() == DeclKind::Using; }
};

//   class DeclState {
//   TemplateParameterList *templateParams;
// public:
//   void SetTemplateParameterList(TemplateParameterList *params);
//   TemplateParameterList *GetTemplateParameterList() const;
// };

// class TemplateDeclState : public DeclState {
// public:
//   // Every DeclState must have a context
//   explicit TemplateDeclState(ASTSession &session);
// };

// class ScopeDeclState : public TemplateDeclState {
// public:
//   // Every DeclState must have a context
//   explicit ScopeDeclState(ASTSession &session);
// };
};

// class TemplateState final : public DeclState {
//   llvm::SmallVector<ParamDecl*, 4> params;

// public:
//   TemplateState(DeclState *parent = nullptr) : DeclState(parent) {}

//   void AddParam(ParamDecl *param) { params.push_back(param); }
//   llvm::ArrayRef<ParamDecl*> GetParams() const { return params; }

//   bool HasParams() const { return !params.empty(); }
// };

// class MacroState final : public DeclState {
//   Identifier macroOrigin;
//   llvm::StringRef macroSource;

// public:
//   MacroState(DeclState *parent = nullptr) : DeclState(parent) {}

//   void SetMacroOrigin(Identifier id) { macroOrigin = id; }
//   Identifier GetMacroOrigin() const { return macroOrigin; }

//   void SetMacroSource(llvm::StringRef src) { macroSource = src; }
//   llvm::StringRef GetMacroSource() const { return macroSource; }

//   bool IsMacroGenerated() const { return !macroSource.empty(); }
// };

// class TemplateState final : public DeclState {
//   llvm::SmallVector<ParamDecl *, 4> params;
//   llvm::SmallVector<Type *, 4> genericArgs;

// public:
//   TemplateState(DeclState *parent = nullptr) : DeclState(parent) {}

//   // -- Parameter handling
//   void AddParam(ParamDecl *param) { params.push_back(param); }
//   llvm::ArrayRef<ParamDecl *> GetParams() const { return params; }
//   bool HasParams() const { return !params.empty(); }

//   // -- Generic arguments (instantiations)
//   void AddGenericArg(Type *arg) { genericArgs.push_back(arg); }
//   llvm::ArrayRef<Type *> GetGenericArgs() const { return genericArgs; }
//   bool IsInstantiated() const { return !genericArgs.empty(); }

//   // -- Overall utility
//   bool IsTemplate() const { return HasParams() || IsInstantiated(); }
// };

// enum class UsingDeclKind : uint8_t {
//   None = 0,
//   Module,
//   Struct,
//   Interface,
//   Enum,
//   Fun,
//   Macro,
// };

// class DotPath {
//   llvm::SmallVector<Identifier, 4> segments;

// public:
//   DotPath() = default;
//   DotPath(llvm::ArrayRef<Identifier> segs)
//       : segments(segs.begin(), segs.end()) {}

//   llvm::ArrayRef<Identifier> GetSegments() const { return segments; }
//   Identifier GetTail() const { return segments.back(); }
//   Identifier GetHead() const { return segments.front(); }

//   void Print(llvm::raw_ostream &os) const;
// };

// class ParametricDeclState : public TemplateDeclState {
//   DotPath dotPath;

// public:
//   void SetDotPath(DotPath path) { dotPath = std::move(path); }
//   const DotPath &GetDotPath() const { return dotPath; }

//   Identifier GetName() const { return dotPath.GetTail(); }
//   llvm::ArrayRef<Identifier> GetPathSegments() const { return
//   dotPath.GetSegments(); }
// };

// class ParametricDeclState : public TemplateDeclState {
//   UsingDeclKind usingKind;
//   DotPath targetPath;                      // optional: the target being
//   aliased llvm::SmallVector<Decl *, 4> typeParams; // optional: for generics
//   like any[T]

// public:
//   ParametricDeclState(UsingDeclKind kind) : usingKind(kind) {}

//   UsingDeclKind GetUsingKind() const { return usingKind; }
//   void SetUsingKind(UsingDeclKind kind) { usingKind = kind; }

//   void SetTargetPath(DotPath path) { targetPath = path; }
//   const DotPath &GetTargetPath() const { return targetPath; }

//   void AddTypeParam(Decl *param) { typeParams.push_back(param); }
//   llvm::ArrayRef<Decl *> GetTypeParams() const { return typeParams; }
// };

// enum class JoinDeclKind : uint8_t {
//   None = 0,
//   Struct,
//   Interface,
//   Enum,
// };

// class JoinDeclState : public ParametricDeclState {
//   JoinDeclKind joinKind;
//   // ...
// };

// void SetJoinKind(JoinDeclKind kind) { joinKind = kind; }
// JoinDeclKind GetJoinKind() { return joinKind; }
// bool IsStruct() const { return joinKind == JoinDeclKind::Struct; }
// bool IsInterface() const { return joinKind == JoinDeclKind::Interface; }
// bool IsEnum() const { return joinKind == JoinDeclKind::Enum; }

// class UsingDeclState : public ParametricDeclState {
//   JoinDeclKind joinKind;
//   // ...
// };

// enum class MacroKind : uint8_t {
//   Expression,
//   Declaration,
//   Statement
// };

// class MacroDeclState : public DeclState {
//   MacroKind macroKind;
//   DotPath name;
//   llvm::SmallVector<ParamDecl *, 4> parameters;

// public:
//   MacroDeclState(MacroKind kind, DotPath name)
//       : macroKind(kind), name(name) {}

//   MacroKind GetMacroKind() const { return macroKind; }
//   DotPath GetNamePath() const { return name; }
//   llvm::ArrayRef<ParamDecl *> GetParameters() const { return parameters; }

//   void AddParam(ParamDecl *param) { parameters.push_back(param); }
// };

} // namespace stone
#endif
