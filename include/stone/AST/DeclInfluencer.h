#ifndef STONE_AST_DECLINFLUENCER_H
#define STONE_AST_DECLINFLUENCER_H

#include "stone/AST/Alignments.h"
#include "stone/AST/Allocation.h"
#include "stone/AST/Attribute.h"
#include "stone/AST/Identifier.h"
#include "stone/AST/Visibility.h"
#include "stone/Core/SrcLoc.h"

#include "llvm/ADT/BitVector.h"
#include "llvm/ADT/DenseMap.h"
#include "llvm/ADT/DenseMapInfo.h"
#include "llvm/ADT/SmallVector.h"
#include "llvm/ADT/StringRef.h"

#include <cassert>
#include <cstdint>
#include <vector>

namespace stone {

/// \brief Enumerates all possible kinds of declaration influencers (modifiers
/// or attributes). These may affect code generation, access control, linkage,
/// behavior, etc.
enum class DeclInfluencerKind : uint8_t {
  None = 0,
#define DECL_INFLUENCER(ID, Parent) ID,
#define LAST_DECL_INFLUENCER(ID) Last_Type = ID,
#define DECL_INFLUENCER_RANGE(ID, FirstID, LastID)                             \
  First_##ID##Type = FirstID, Last_##ID##Type = LastID,
#include "stone/AST/DeclInfluencerNode.def"
};

} // namespace stone

/// \brief Hashing and equality utilities for using DeclInfluencerKind in
/// DenseMap.
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

/// \brief Base class for any AST construct that modifies a declaration’s
/// behavior or semantics.
class alignas(1 << DeclAlignInBits) DeclInfluencer
    : public Allocation<DeclInfluencer> {
  DeclInfluencerKind kind;
  SrcLoc loc;

public:
  DeclInfluencer(DeclInfluencerKind kind, SrcLoc loc) : kind(kind), loc(loc) {}

  DeclInfluencerKind GetKind() const { return kind; }
  SrcLoc GetLoc() const { return loc; }

  /// Convenience checkers for common modifiers/attributes
  bool IsTrust() const { return kind == DeclInfluencerKind::Trust; }
  bool IsOpen() const { return kind == DeclInfluencerKind::Open; }
  bool IsVirtual() const { return kind == DeclInfluencerKind::Virtual; }
  bool IsPersonal() const { return kind == DeclInfluencerKind::Personal; }
  bool IsVisibility() const { return kind == DeclInfluencerKind::Visibility; }
  bool IsDeprecated() const { return kind == DeclInfluencerKind::Deprecated; }
  bool IsIntrinsic() const { return kind == DeclInfluencerKind::Intrinsic; }
  bool IsInline() const { return kind == DeclInfluencerKind::Inline; }
  bool IsHardware() const { return kind == DeclInfluencerKind::Hardware; }
  bool IsExclusive() const { return kind == DeclInfluencerKind::Exclusive; }
};

//===----------------------------------------------------------------------===//
// Modifier subclasses
//===----------------------------------------------------------------------===//

/// Base class for user-declared modifiers.
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

class OpenModifier final : public DeclModifier {
public:
  OpenModifier(SrcLoc loc) : DeclModifier(DeclInfluencerKind::Open, loc) {}
};

class HardwareModifier : public DeclModifier {
public:
  HardwareModifier(SrcLoc loc)
      : DeclModifier(DeclInfluencerKind::Hardware, loc) {}
};

class ExclusiveModifier : public DeclModifier {
public:
  ExclusiveModifier(SrcLoc loc)
      : DeclModifier(DeclInfluencerKind::Exclusive, loc) {}
};

/// \brief Modifier to control declaration visibility (public/local).
class VisibilityModifier final : public DeclModifier {
  VisibilityLevel level;

public:
  VisibilityModifier(VisibilityLevel level, SrcLoc loc)
      : DeclModifier(DeclInfluencerKind::Visibility, loc), level(level) {}

  VisibilityLevel GetLevel() const { return level; }
  void SetLevel(VisibilityLevel lvl) { level = lvl; }

  void MakePublic() { SetLevel(VisibilityLevel::Public); }
  void MakeLocal() { SetLevel(VisibilityLevel::Local); }
};

//===----------------------------------------------------------------------===//
// Attribute subclasses
//===----------------------------------------------------------------------===//

/// \brief A DeclAttribute is a source-attached property that modifies semantic
/// meaning. Unlike modifiers, attributes can carry additional data (like source
/// ranges).
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

class ExternAttribute : public DeclAttribute {
public:
  ExternAttribute(SrcLoc loc, SrcRange range)
      : DeclAttribute(DeclInfluencerKind::Extern, loc, range) {}
};

/// \brief Enumerates all supported compiler intrinsics.
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
  Unreachable
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

//===----------------------------------------------------------------------===//
// DeclInfluencerList Infrastructure
//===----------------------------------------------------------------------===//

/// \brief Abstract base class to track a set of declaration influencers.
/// Influencers are stored in a map with a fast bitvector for presence checking.
class AbstractDeclInfluencerList {
  llvm::BitVector mask;
  llvm::DenseMap<DeclInfluencerKind, DeclInfluencer *> influencers;

public:
  AbstractDeclInfluencerList()
      : mask(static_cast<unsigned>(DeclInfluencerKind::Last_Type) + 1) {}

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

  bool IsEmpty() const { return influencers.empty(); }
};

/// \brief Concrete influencer list for declarations.
/// Provides typed add/has methods for each known influencer kind.
class DeclInfluencerList final : public AbstractDeclInfluencerList {
public:
  explicit DeclInfluencerList() {}

  void AddTrust(TrustModifier *modifier) { Add(modifier); }
  void AddPure(PureModifier *modifier) { Add(modifier); }
  void AddVirtual(VirtualModifier *modifier) { Add(modifier); }
  void AddPersonal(PersonalModifier *modifier) { Add(modifier); }

  bool HasTrust() { return Has(DeclInfluencerKind::Trust); }
  bool HasPure() { return Has(DeclInfluencerKind::Pure); }
  bool HasVirtual() { return Has(DeclInfluencerKind::Virtual); }
  bool HasPersonal() { return Has(DeclInfluencerKind::Personal); }
  bool HasVisibility() { return Has(DeclInfluencerKind::Visibility); }
  bool HasHardware() const { return Has(DeclInfluencerKind::Hardware); }
  bool HasExclusive() const { return Has(DeclInfluencerKind::Exclusive); }

  void AddIntrinsic(IntrinsicAttribute *attribute) { Add(attribute); }
  void AddExtern(ExternAttribute *attribute) { Add(attribute); }

  bool HasIntrinsic() { return Has(DeclInfluencerKind::Intrinsic); }
  bool HasExtern() { return Has(DeclInfluencerKind::Extern); }
};

} // namespace stone

#endif // STONE_AST_DECLINFLUENCER_H
