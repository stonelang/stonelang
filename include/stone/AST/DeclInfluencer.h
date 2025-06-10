#ifndef STONE_AST_DECLINFLUENCER_H
#define STONE_AST_DECLINFLUENCER_H

#include "stone/AST/AST.h"
#include "stone/AST/Attribute.h"
#include "stone/AST/Identifier.h"
#include "stone/AST/TypeAlignment.h"
#include "stone/AST/Visibility.h"
#include "stone/Core/SrcLoc.h"

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

  // Convenience checks
  bool IsTrust() const { return kind == DeclInfluencerKind::Trust; }
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

} // namespace stone

#endif
