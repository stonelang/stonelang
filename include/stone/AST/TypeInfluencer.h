#ifndef STONE_AST_TYPEINFLUENCER_H
#define STONE_AST_TYPEINFLUENCER_H

#include "stone/AST/Attribute.h"
#include "stone/AST/Identifier.h"
#include "stone/AST/TypeAlignment.h"
#include "stone/AST/Visibility.h"
#include "stone/Support/SrcLoc.h"

#include "llvm/ADT/BitVector.h"
#include "llvm/ADT/DenseMap.h"
#include "llvm/ADT/DenseMapInfo.h"
#include "llvm/ADT/SmallVector.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/ADT/iterator_range.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/TrailingObjects.h"
#include "llvm/Support/VersionTuple.h"

#include <cassert>
#include <cstdint>
#include <stdint.h>
#include <vector>

namespace stone {

enum class TypeInfluencerKind : uint8_t {
  None = 0, // Default value for uninitialized or null influencers
#define TYPE_INFLUENCER(ID, Parent) ID,
#define LAST_TYPE_INFLUENCER(ID) Last_Type = ID,
#define TYPE_INFLUENCER_RANGE(ID, FirstID, LastID)                             \
  First_##ID##Type = FirstID, Last_##ID##Type = LastID,
#include "stone/AST/TypeInfluencerNode.def"
};

} // namespace stone

namespace llvm {

template <> struct DenseMapInfo<stone::TypeInfluencerKind> {

  static inline stone::TypeInfluencerKind getEmptyKey() {
    return stone::TypeInfluencerKind::None;
  }
  static inline stone::TypeInfluencerKind getTombstoneKey() {
    return static_cast<stone::TypeInfluencerKind>(
        static_cast<uint8_t>(stone::TypeInfluencerKind::Last_Type) + 1);
  }
  static unsigned getHashValue(stone::TypeInfluencerKind kind) {
    return static_cast<unsigned>(kind);
  }
  static bool isEqual(stone::TypeInfluencerKind lhs,
                      stone::TypeInfluencerKind rhs) {
    return lhs == rhs;
  }
};

} // namespace llvm

namespace stone {
class alignas(1 << TypeAlignInBits) TypeInfluencer
    : public ASTAllocation<TypeInfluencer> {
  TypeInfluencerKind kind;
  SrcLoc loc;

public:
  TypeInfluencer(TypeInfluencerKind kind, SrcLoc loc) : kind(kind), loc(loc) {}

public:
  TypeInfluencerKind GetKind() { return kind; }
  SrcLoc GetLoc() { return loc; }

public:
  bool IsStone() { return GetKind() == TypeInfluencerKind::Stone; }
  bool IsAlign() { return GetKind() == TypeInfluencerKind::Align; }
  bool IsNullable() { return GetKind() == TypeInfluencerKind::Nullable; }
  bool IsNotNullable() { return GetKind() == TypeInfluencerKind::NotNullable; }
  bool IsOwn() { return GetKind() == TypeInfluencerKind::Own; }
  bool IsSafe() { return GetKind() == TypeInfluencerKind::Safe; }
  bool IsNot() { return GetKind() == TypeInfluencerKind::Not; }
};

class TypeModifier : public TypeInfluencer {
public:
  TypeModifier(TypeInfluencerKind kind, SrcLoc loc)
      : TypeInfluencer(kind, loc) {}
};

class StoneModifier : public TypeModifier {
public:
  StoneModifier(SrcLoc loc) : TypeModifier(TypeInfluencerKind::Stone, loc) {}
};

class OwnModifier : public TypeModifier {
public:
  OwnModifier(SrcLoc loc) : TypeModifier(TypeInfluencerKind::Own, loc) {}
};
class SafeModifier : public TypeModifier {
public:
  SafeModifier(SrcLoc loc) : TypeModifier(TypeInfluencerKind::Safe, loc) {}
};
class NotModifier : public TypeModifier {
public:
  NotModifier(SrcLoc loc) : TypeModifier(TypeInfluencerKind::Not, loc) {}
};

class TypeAttribute : public TypeInfluencer, public Attribute {
public:
  TypeAttribute(TypeInfluencerKind kind, SrcLoc lBracketLoc, SrcRange range)
      : TypeInfluencer(kind, lBracketLoc), Attribute(lBracketLoc, range) {}
};

class NullableAttribute : public TypeAttribute {
public:
  NullableAttribute(SrcLoc lBracketLoc, SrcRange range)
      : TypeAttribute(TypeInfluencerKind::Nullable, lBracketLoc, range) {}
};

class AlignAttribute : public TypeAttribute {
public:
  AlignAttribute(SrcLoc lBracketLoc, SrcRange range)
      : TypeAttribute(TypeInfluencerKind::Align, lBracketLoc, range) {}
};

class AbstractTypeInfluencerList {
  llvm::BitVector mask;
  llvm::DenseMap<TypeInfluencerKind, TypeInfluencer *> influencers;

public:
  AbstractTypeInfluencerList()
      : mask(static_cast<unsigned>(TypeInfluencerKind::Last_Type) + 1) {}

  explicit operator bool() const { return !influencers.empty(); }

public:
  void Add(TypeInfluencer *influencer) {
    influencers[influencer->GetKind()] = influencer;
    mask.set(static_cast<unsigned>(influencer->GetKind()));
  }
  bool Has(TypeInfluencerKind kind) const {
    return mask.test(static_cast<unsigned>(kind));
  }
  bool Has(TypeInfluencer *influencer) const {
    assert(influencer && "Cannot add null type-influencer!");
    return Has(influencer->GetKind());
  }

  TypeInfluencer *Get(TypeInfluencerKind kind) const {
    auto it = influencers.find(kind);
    return it != influencers.end() ? it->second : nullptr;
  }
  bool IsEmpty() const { return influencers.size() == 0; }
};

class TypeInfluencerList final : public AbstractTypeInfluencerList {
  const ASTSession &session;

public:
  explicit TypeInfluencerList(const ASTSession &session) : session(session) {}

public:
  const ASTSession &GetASTSession() { return session; }

public:
  void AddStone(SrcLoc loc) { Add(new (session) StoneModifier(loc)); }
  void AddOwn(SrcLoc loc) { Add(new (session) OwnModifier(loc)); }
  void AddSafe(SrcLoc loc) { Add(new (session) SafeModifier(loc)); }
  void AddNot(SrcLoc loc) { Add(new (session) NotModifier(loc)); }

public:
  bool HasStone() const { return Has(TypeInfluencerKind::Stone); }
  bool HasOwn() const { return Has(TypeInfluencerKind::Own); }
  bool HasSafe() const { return Has(TypeInfluencerKind::Safe); }
  bool HasNot() const { return Has(TypeInfluencerKind::Not); }

public:
};

} // namespace stone
#endif
