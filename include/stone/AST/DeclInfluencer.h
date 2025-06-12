#ifndef STONE_AST_DECLINFLUENCER_H
#define STONE_AST_DECLINFLUENCER_H

#include "stone/AST/AST.h"
#include "stone/AST/Attribute.h"
#include "stone/AST/Identifier.h"
#include "stone/AST/TypeAlignment.h"
#include "stone/AST/Visibility.h"
#include "stone/Lang/SrcLoc.h"

#include "llvm/ADT/SmallVector.h"
#include "llvm/ADT/StringRef.h"

#include <cassert>
#include <cstdint>
#include <vector>
namespace stone {

enum class DeclInfluencerKind : uint8_t {
  None = 0, // Default value for uninitialized or null properties
#define DECL_INFLUENCER(ID, Parent) ID,
#define LAST_DECL_INFLUENCER(ID) Last_Type = ID,
#define DECL_INFLUENCER_RANGE(ID, FirstID, LastID)                             \
  First_##ID##Type = FirstID, Last_##ID##Type = LastID,
#include "stone/AST/DeclInfluencerNode.def"
};
} // namespace stone

namespace llvm {

template <> struct DenseMapInfo<stone::DeclInfluencerKind> {

  static inline stone::DeclInfluencerKind getEmptyKey() {
    return stone::DeclInfluencerKind::None;
  }
  static inline stone::DeclInfluencerKind getTombstoneKey() {
    return static_cast<stone::DeclInfluencerKind>(
        static_cast<uint8_t>(stone::DeclInfluencerKind::Last_Type) + 1);
  }
  static unsigned getHashValue(stone::DeclInfluencerKind kind) {
    return static_cast<unsigned>(kind);
  }
  static bool isEqual(stone::DeclInfluencerKind lhs,
                      stone::DeclInfluencerKind rhs) {
    return lhs == rhs;
  }
};

} // namespace llvm

namespace stone {

class alignas(1 << DeclAlignInBits) DeclInfluencer
    : public ASTAllocation<DeclInfluencer> {
  DeclInfluencerKind kind;
  SrcLoc loc;

public:
  DeclInfluencer(DeclInfluencerKind kind, SrcLoc loc) : kind(kind), loc(loc) {}

public:
  DeclInfluencerKind GetKind() const { return kind; }
  SrcLoc GetLoc() const { return loc; }

public:
  // Convenience checks
  bool IsTrust() const { return kind == DeclInfluencerKind::Trust; }
  bool IsOpen() const { return kind == DeclInfluencerKind::Open; }
  bool IsVirtual() const { return kind == DeclInfluencerKind::Virtual; }
  bool IsPersonal() const { return kind == DeclInfluencerKind::Personal; }
  bool IsVisibility() const { return kind == DeclInfluencerKind::Visibility; }
  bool IsDeprecated() const { return kind == DeclInfluencerKind::Deprecated; }
  bool IsIntrinsic() const { return kind == DeclInfluencerKind::Intrinsic; }
  bool IsInline() const { return kind == DeclInfluencerKind::Inline; }
};

// --------------------
// DeclModifier Hierarchy
// --------------------
class DeclModifier : public DeclInfluencer {
public:
  DeclModifier(DeclInfluencerKind kind, SrcLoc loc)
      : DeclInfluencer(kind, loc) {}
};

class TrustModifier : public DeclModifier {
public:
  TrustModifier(SrcLoc loc) : DeclModifier(DeclInfluencerKind::Trust, loc) {}
};

class PureModifier : public DeclModifier {
public:
  PureModifier(SrcLoc loc) : DeclModifier(DeclInfluencerKind::Pure, loc) {}
};

class VirtualModifier : public DeclModifier {
public:
  VirtualModifier(SrcLoc loc)
      : DeclModifier(DeclInfluencerKind::Virtual, loc) {}
};

class PersonalModifier : public DeclModifier {
public:
  PersonalModifier(SrcLoc loc)
      : DeclModifier(DeclInfluencerKind::Personal, loc) {}
};

class OpenModifier : public DeclModifier {
public:
  OpenModifier(SrcLoc loc) : DeclModifier(DeclInfluencerKind::Open, loc) {}
};

class VisibilityModifier : public DeclModifier {
  VisibilityLevel level;

public:
  VisibilityModifier(VisibilityLevel level, SrcLoc loc)
      : DeclModifier(DeclInfluencerKind::Visibility, loc), level(level) {}

  VisibilityLevel GetLevel() const { return level; }
  void SetLevel(VisibilityLevel lvl) { level = lvl; }
};

// --------------------
// DeclAttribute Hierarchy
// --------------------
class DeclAttribute : public DeclInfluencer, public Attribute {
public:
  DeclAttribute(DeclInfluencerKind kind, SrcLoc loc, SrcRange range)
      : DeclInfluencer(kind, loc), Attribute(loc, range) {}
};

class DeprecatedAttribute : public DeclAttribute {
public:
  DeprecatedAttribute(SrcLoc loc, SrcRange range)
      : DeclAttribute(DeclInfluencerKind::Deprecated, loc, range) {}
};

class InlineAttribute : public DeclAttribute {
public:
  InlineAttribute(SrcLoc loc, SrcRange range)
      : DeclAttribute(DeclInfluencerKind::Inline, loc, range) {}
};

enum class IntrinsicKind : uint8_t {
  None = 0,
  PopCount,
  CountLeadingZeros,
  CountTrailingZeros,
  BitReverse,

  SaturatingAddSigned,
  SaturatingAddUnsigned,
  FusedMulAdd,
  SqrtFloat,
  SqrtDouble,

  Memcpy,
  Memmove,
  Memset,
  MemoryFence,

  AtomicAdd,
  AtomicSub,
  AtomicAnd,
  AtomicOr,
  AtomicXor,
  AtomicCompareExchange,

  Expect,
  Trap,
  DebugTrap,
  Unreachable,

  // (optional future: target-specific)
  // X86SSE2Sqrt,
  // ARMNEONAdd
};

class IntrinsicAttribute : public DeclAttribute {
  IntrinsicKind intrinsicKind;

public:
  IntrinsicAttribute(IntrinsicKind kind, SrcLoc loc, SrcRange range)
      : DeclAttribute(DeclInfluencerKind::Intrinsic, loc, range),
        intrinsicKind(kind) {}

  IntrinsicKind GetIntrinsicKind() const { return intrinsicKind; }
  void SetIntrinsicKind(IntrinsicKind kind) { intrinsicKind = kind; }
};

class AbstractDeclInfluencerList {
  llvm::BitVector mask;
  llvm::DenseMap<DeclInfluencerKind, DeclInfluencer *> influencers;

public:
  AbstractDeclInfluencerList()
      : mask(static_cast<unsigned>(DeclInfluencerKind::Last_Type) + 1) {}

  // Overload the bool operator
  explicit operator bool() const { return !influencers.empty(); }

protected:
  void Add(DeclInfluencer *influencer) {
    influencers[influencer->GetKind()] = influencer;
    mask.set(static_cast<unsigned>(influencer->GetKind()));
  }
  bool Has(DeclInfluencerKind kind) const {
    return mask.test(static_cast<unsigned>(kind));
  }
  bool Has(DeclInfluencer *influencer) const {
    assert(influencer && "Cannot add null decl-influencer!");
    return Has(influencer->GetKind());
  }

public:
  DeclInfluencer *Get(DeclInfluencerKind kind) const {
    auto it = influencers.find(kind);
    return it != influencers.end() ? it->second : nullptr;
  }

public:
  bool IsEmpty() const { return influencers.size() == 0; }
};

class DeclInfluencerList final : public AbstractDeclInfluencerList {
  ASTSession &session;

public:
  explicit DeclInfluencerList(ASTSession &session) : session(session) {}

public:
  ASTSession &GetASTSession() { return session; }

public:
  void AddTrust(SrcLoc loc) {
    Add(new (session) TrustModifier(DeclInfluencerKind::Trust, loc));
  }
  void AddPure(SrcLoc loc) {
    Add(new (session) TrustModifier(DeclInfluencerKind::Pure, loc));
  }
  void AddVirtual(SrcLoc loc) {
    Add(new (session) TrustModifier(DeclInfluencerKind::Virtual, loc));
  }
  void AddPersonal(SrcLoc loc) {
    Add(new (session) TrustModifier(DeclInfluencerKind::Personal, loc));
  }
  void AddPublic(SrcLoc loc) {
    Add(new (session) VisibilityModifier(VisibilityLevel::Public, loc));
  }
  void AddPrivate(SrcLoc loc) {
    Add(new (session) VisibilityModifier(VisibilityLevel::Private, loc));
  }
  void AddInternal(SrcLoc loc) {
    Add(new (session) VisibilityModifier(VisibilityLevel::Internal, loc));
  }
  void AddExtern(SrcLoc loc) { Add(new (session) ExternModifier(loc)); }

public:
  bool HasTrust() { return Has(DeclInfluencerKind::Trust); }
  bool HasPure() { return Has(DeclInfluencerKind::Pure); }
  bool HasVritual() { return Has(DeclInfluencerKind::Virtual); }
  bool HasPersonal() { return Has(DeclInfluencerKind::Personal); }
  bool HasVisibility() { return Has(DeclInfluencerKind::Visibility); }
  bool HasExtern() { return Has(DeclInfluencerKind::Extern); }

public:
  void AddAlign(SrcLoc loc) {
    Add(new (session) AlignAttribute(DeclInfluencerKind::Align, loc));
  }
  void AddIntrinsic(SrcLoc loc) {
    Add(new (session) AlignAttribute(DeclInfluencerKind::Intrinsic, loc));
  }

public:
  bool HasAlign() { return Has(DeclInfluencerKind::Align); }
  bool HasIntrinsic() { return Has(DeclInfluencerKind::Intrinsic); }
};

} // namespace stone

#endif
