#ifndef STONE_AST_TYPEINFLUENCER_H
#define STONE_AST_TYPEINFLUENCER_H

//===----------------------------------------------------------------------===//
// Includes
//===----------------------------------------------------------------------===//

#include "stone/AST/Alignments.h"
#include "stone/AST/Allocation.h"
#include "stone/AST/Attribute.h"
#include "stone/AST/Identifier.h"
#include "stone/AST/Memory.h"
#include "stone/AST/Visibility.h"
#include "stone/Core/SrcLoc.h"

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

//===----------------------------------------------------------------------===//
// TypeInfluencerKind
// Enumerates all modifier/attribute kinds that can influence a type's behavior.
//===----------------------------------------------------------------------===//

enum class TypeInfluencerKind : uint8_t {
  None = 0,
#define TYPE_INFLUENCER(ID, Parent) ID,
#define LAST_TYPE_INFLUENCER(ID) Last_Type = ID,
#define TYPE_INFLUENCER_RANGE(ID, FirstID, LastID)                             \
  First_##ID##Type = FirstID, Last_##ID##Type = LastID,
#include "stone/AST/TypeInfluencerNode.def"
};

} // namespace stone

//===----------------------------------------------------------------------===//
// DenseMapInfo specialization for TypeInfluencerKind
// Allows use of TypeInfluencerKind as DenseMap keys.
//===----------------------------------------------------------------------===//

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

//===----------------------------------------------------------------------===//
// TypeInfluencer
// Base class representing a modifier or attribute that influences a type.
//===----------------------------------------------------------------------===//

class alignas(1 << TypeAlignInBits) TypeInfluencer
    : public Allocation<TypeInfluencer> {
  TypeInfluencerKind kind;
  SrcLoc loc;

public:
  TypeInfluencer(TypeInfluencerKind kind, SrcLoc loc) : kind(kind), loc(loc) {}

  TypeInfluencerKind GetKind() { return kind; }
  SrcLoc GetLoc() { return loc; }

  // Query helpers
  bool IsStone() const { return kind == TypeInfluencerKind::Stone; }
  bool IsAlign() const { return kind == TypeInfluencerKind::Align; }
  bool IsNullable() const { return kind == TypeInfluencerKind::Nullable; }
  bool IsNotNullable() const { return kind == TypeInfluencerKind::NotNullable; }
  bool IsOwn() const { return kind == TypeInfluencerKind::Own; }
  bool IsSafe() const { return kind == TypeInfluencerKind::Safe; }
  bool IsNot() const { return kind == TypeInfluencerKind::Not; }
};

//===----------------------------------------------------------------------===//
// Modifiers (syntax like: `own ptr int`)
//===----------------------------------------------------------------------===//

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

//===----------------------------------------------------------------------===//
// Attributes (syntax like: `[nullable]`, `[align]`)
//===----------------------------------------------------------------------===//

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

//===----------------------------------------------------------------------===//
// AbstractTypeInfluencerList
// Stores a mask and mapping of all present influencers.
//===----------------------------------------------------------------------===//

class AbstractTypeInfluencerList {
  llvm::BitVector mask;
  llvm::DenseMap<TypeInfluencerKind, TypeInfluencer *> influencers;

public:
  AbstractTypeInfluencerList()
      : mask(static_cast<unsigned>(TypeInfluencerKind::Last_Type) + 1) {}

  explicit operator bool() const { return !influencers.empty(); }

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

  bool IsEmpty() const { return influencers.empty(); }
};

//===----------------------------------------------------------------------===//
// TypeInfluencerList
// Concrete collection interface for managing type modifiers/attributes.
//===----------------------------------------------------------------------===//

class TypeInfluencerList final : public AbstractTypeInfluencerList {
public:
  TypeInfluencerList() = default;

  void AddStone(StoneModifier *modifier) { Add(modifier); }
  void AddOwn(OwnModifier *modifier) { Add(modifier); }
  void AddSafe(SafeModifier *modifier) { Add(modifier); }
  void AddNot(NotModifier *modifier) { Add(modifier); }

  bool HasStone() const { return Has(TypeInfluencerKind::Stone); }
  bool HasOwn() const { return Has(TypeInfluencerKind::Own); }
  bool HasSafe() const { return Has(TypeInfluencerKind::Safe); }
  bool HasNot() const { return Has(TypeInfluencerKind::Not); }
};

} // namespace stone

#endif // STONE_AST_TYPEINFLUENCER_H
