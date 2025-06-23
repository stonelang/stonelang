#ifndef STONE_AST_DECLSTATE_H
#define STONE_AST_DECLSTATE_H

#include "stone/AST/DeclInfluencer.h"
#include "stone/AST/DeclKind.h"
#include "stone/AST/Identifier.h"
#include "stone/AST/Allocation.h"
#include "stone/AST/ParamList.h"
#include "stone/AST/Alignments.h"
#include "stone/AST/TypeFlight.h"
#include "stone/AST/TypeInfluencer.h"

#include "llvm/ADT/BitVector.h"
#include "llvm/ADT/DenseMap.h"
#include "llvm/Support/Allocator.h"

#include <cassert>

namespace stone {
class Decl;
class DeclFlight;
class TypeFlight;
class ASTSession;
class ParamList;

// enum class DeclFlightKind : uint8_t {
//   Parametric,
//   Mutation,
//   Using,
//   Join,
//   Builtin,
//   Function,
//   Alias
// };

enum class DeclFlightKind : uint8_t {
  Builtin,
  Function,
};

class alignas(1 << DeclAlignInBits) DeclFlight
    : public Allocation<DeclFlight> {
  // friend Decl;
  Memory &mem;

  // The declaration kind
  DeclKind kind = DeclKind::None;

  // Source location for the decl kind
  SrcLoc kindLoc;

  // The declaration name
  Identifier name;

  // Source location for the decl name.
  SrcLoc nameLoc;

  // The Decl associated with this DeclFlight
  Decl *owner = nullptr;

  // The parent decl state -- maybe not
  DeclFlight *parent = nullptr;

  // The properties for the DeclFlight
  DeclInfluencerList declInfluencerList;

  // The properties for the DeclFlight
  TypeInfluencerList typeInfluencerList;

  ParamList *paramList;

public:
  // Direct comparison is disabled for states
  void operator==(DeclFlight D) const = delete;
  void operator!=(DeclFlight D) const = delete;

  // Every DeclFlight must have a context
  explicit DeclFlight(Memory &mem, DeclFlight *parent = nullptr);

public:
  void SetKind(DeclKind K) { kind = K; }
  DeclKind GetKind() const { return kind; }

  SrcLoc GetKindLoc() { return nameLoc; }
  void SetKindLoc(SrcLoc currentLoc) { nameLoc = currentLoc; }

  void SetName(Identifier I) { name = I; }
  Identifier GetName() { return name; }

  SrcLoc GetNameLoc() const { return nameLoc; }
  void SetNameLoc(SrcLoc currentLoc) { nameLoc = currentLoc; }

  void SetOwner(Decl *D) { owner = D; }
  Decl *GetOwner() { return owner; }
  bool HasOwner() { return owner != nullptr; }

  void SetParent(DeclFlight *P) { parent = P; }
  DeclFlight *GetParent() { return parent; }
  bool HasParent() { return parent != nullptr; }

  TypeInfluencerList &GetTypeInfluencerList() { return typeInfluencerList; }
  DeclInfluencerList &GetDeclInfluencerList() { return declInfluencerList; }

  void SetParamList(ParamList *PL) { paramList = PL; }
  ParamList *GetParamList() { return paramList; }
  Memory &GetMemory() { return mem; }

  // template <typename T>
  // T *GetParamListAs() const {
  //   return llvm::dyn_cast<T>(params);
  // }

  //   oid SetParamList(std::unique_ptr<ParamList> p) {
  //   switch (p->GetKind()) {
  //     case ParamListKind::Any:
  //       assert(isa<MutationDecl>(thisDecl) && "Only MutationDecl can have
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

  virtual DeclFlightKind GetDeclFlightKind() const = 0;

public:
  bool IsJoin() { return GetKind() == DeclKind::Join; }
  bool IsSpace() { return GetKind() == DeclKind::Space; }
  bool IsUsing() { return GetKind() == DeclKind::Using; }
};

class ParametricDeclFlight : public DeclFlight {
public:
};

class SpaceDeclFlight : public ParametricDeclFlight {
public:
  SpaceDeclFlight();
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

class UsingDeclFlight : public ParametricDeclFlight {
  UsingDeclKind kind;

public:
  UsingDeclFlight();
};

enum class JoinDeclKind : uint8_t {
  None = 0,
  Struct,
  Interface,
  Enum,
};

class JoinDeclFlight : public DeclFlight {
  JoinDeclKind joinKind;
};

// class ScopeDeclFlight : public DeclFlight {
// public:
//   explicit ScopeDeclFlight(DeclFlight *parent) : DeclFlight(parent) {}
// };

class SomeDeclFlight : public ParametricDeclFlight {
public:
};

class JustDeclFlight : public ParametricDeclFlight {
public:
};

enum class VarDeclKind { None = 0, Local, Param, Global, Field };

class VarDeclFlight : public DeclFlight {};

//   class DeclFlight {
//   MutationParameterList *templateParams;
// public:
//   void SetMutationParameterList(MutationParameterList *params);
//   MutationParameterList *GetMutationParameterList() const;
// };

// class MutationDeclFlight : public DeclFlight {
// public:
//   // Every DeclFlight must have a context
//   explicit MutationDeclFlight(ASTSession &session);
// };

// class ScopeDeclFlight : public MutationDeclFlight {
// public:
//   // Every DeclFlight must have a context
//   explicit ScopeDeclFlight(ASTSession &session);
// };
//};

// class MutationDeclFlight final : public DeclFlight {
//   llvm::SmallVector<ParamDecl*, 4> params;

// public:
//   MutationDeclFlight(DeclFlight *parent = nullptr) : DeclFlight(parent) {}

//   void AddParam(ParamDecl *param) { params.push_back(param); }
//   llvm::ArrayRef<ParamDecl*> GetParams() const { return params; }

//   bool HasParams() const { return !params.empty(); }
// };

// class MacroState final : public DeclFlight {
//   Identifier macroOrigin;
//   llvm::StringRef macroSource;

// public:
//   MacroState(DeclFlight *parent = nullptr) : DeclFlight(parent) {}

//   void SetMacroOrigin(Identifier id) { macroOrigin = id; }
//   Identifier GetMacroOrigin() const { return macroOrigin; }

//   void SetMacroSource(llvm::StringRef src) { macroSource = src; }
//   llvm::StringRef GetMacroSource() const { return macroSource; }

//   bool IsMacroGenerated() const { return !macroSource.empty(); }
// };

// class MutationDeclFlight final : public DeclFlight {
//   llvm::SmallVector<ParamDecl *, 4> params;
//   llvm::SmallVector<Type *, 4> genericArgs;

// public:
//   MutationDeclFlight(DeclFlight *parent = nullptr) : DeclFlight(parent) {}

//   // -- Parameter handling
//   void AddParam(ParamDecl *param) { params.push_back(param); }
//   llvm::ArrayRef<ParamDecl *> GetParams() const { return params; }
//   bool HasParams() const { return !params.empty(); }

//   // -- Generic arguments (instantiations)
//   void AddGenericArg(Type *arg) { genericArgs.push_back(arg); }
//   llvm::ArrayRef<Type *> GetGenericArgs() const { return genericArgs; }
//   bool IsInstantiated() const { return !genericArgs.empty(); }

//   // -- Overall utility
//   bool IsMutation() const { return HasParams() || IsInstantiated(); }
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

// class ParametricDeclFlight : public MutationDeclFlight {
//   DotPath dotPath;

// public:
//   void SetDotPath(DotPath path) { dotPath = std::move(path); }
//   const DotPath &GetDotPath() const { return dotPath; }

//   Identifier GetName() const { return dotPath.GetTail(); }
//   llvm::ArrayRef<Identifier> GetPathSegments() const { return
//   dotPath.GetSegments(); }
// };

// class ParametricDeclFlight : public MutationDeclFlight {
//   UsingDeclKind usingKind;
//   DotPath targetPath;                      // optional: the target being
//   aliased llvm::SmallVector<Decl *, 4> typeParams; // optional: for generics
//   like any[T]

// public:
//   ParametricDeclFlight(UsingDeclKind kind) : usingKind(kind) {}

//   UsingDeclKind GetUsingKind() const { return usingKind; }
//   void SetUsingKind(UsingDeclKind kind) { usingKind = kind; }

//   void SetTargetPath(DotPath path) { targetPath = path; }
//   const DotPath &GetTargetPath() const { return targetPath; }

//   void AddTypeParam(Decl *param) { typeParams.push_back(param); }
//   llvm::ArrayRef<Decl *> GetTypeParams() const { return typeParams; }
// };

// void SetJoinKind(JoinDeclKind kind) { joinKind = kind; }
// JoinDeclKind GetJoinKind() { return joinKind; }
// bool IsStruct() const { return joinKind == JoinDeclKind::Struct; }
// bool IsInterface() const { return joinKind == JoinDeclKind::Interface; }
// bool IsEnum() const { return joinKind == JoinDeclKind::Enum; }

// class UsingDeclFlight : public ParametricDeclFlight {
//   JoinDeclKind joinKind;
//   // ...
// };

// enum class MacroKind : uint8_t {
//   Expression,
//   Declaration,
//   Statement
// };

// class MacroDeclFlight : public DeclFlight {
//   MacroKind macroKind;
//   DotPath name;
//   llvm::SmallVector<ParamDecl *, 4> parameters;

// public:
//   MacroDeclFlight(MacroKind kind, DotPath name)
//       : macroKind(kind), name(name) {}

//   MacroKind GetMacroKind() const { return macroKind; }
//   DotPath GetNamePath() const { return name; }
//   llvm::ArrayRef<ParamDecl *> GetParameters() const { return parameters; }

//   void AddParam(ParamDecl *param) { parameters.push_back(param); }
// };

} // namespace stone
#endif
