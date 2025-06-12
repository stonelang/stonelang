#ifndef STONE_AST_DECLNAME_H
#define STONE_AST_DECLNAME_H

#include "stone/AST/Identifier.h"

#include "llvm/ADT/ArrayRef.h"
#include "llvm/ADT/FoldingSet.h"
#include "llvm/ADT/PointerUnion.h"
#include "llvm/Support/TrailingObjects.h"

namespace stone {

enum class DeclNameKind : uint8_t {
  Basic = 0,
  Constructor,
  Destructor,
  Operator,
  LiteralOperator,
  UsingDirective,
  Compound
};

class DeclNameBase final {
  Identifier identifier;

public:
  DeclNameBase() : DeclNameBase(Identifier()) {}
  DeclNameBase(Identifier I) : identifier(I) {}

  bool IsValid() const { return static_cast<bool>(identifier); }
  Identifier GetIdentifier() const { return identifier; }

  bool operator==(DeclNameBase rhs) const {
    return identifier == rhs.identifier;
  }
  bool operator!=(DeclNameBase rhs) const { return !(*this == rhs); }
  bool operator<(DeclNameBase rhs) const {
    return identifier.GetPointer() < rhs.identifier.GetPointer();
  }

  const void *GetAsOpaquePointer() const { return identifier.GetPointer(); }
  static DeclNameBase GetFromOpaquePointer(void *ptr) {
    return DeclNameBase(Identifier::GetFromOpaquePointer(ptr));
  }
};

/// Represents a compound declaration name.
struct alignas(void *) CompoundDeclName final
    : llvm::FoldingSetNode,
      private llvm::TrailingObjects<CompoundDeclName, Identifier> {

  friend TrailingObjects;
  friend class DeclName;

  DeclNameBase nameBase;
  size_t numArgs;

  CompoundDeclName(DeclNameBase base, size_t numArgs)
      : nameBase(base), numArgs(numArgs) {}

  llvm::ArrayRef<Identifier> GetArgumentNames() const {
    return {getTrailingObjects<Identifier>(), numArgs};
  }

  static void Profile(llvm::FoldingSetNodeID &id, DeclNameBase base,
                      llvm::ArrayRef<Identifier> args) {
    id.AddPointer(base.GetIdentifier().GetPointer());
    for (auto arg : args) {
      id.AddPointer(arg.GetPointer());
    }
  }

  void Profile(llvm::FoldingSetNodeID &id) {
    Profile(id, nameBase, GetArgumentNames());
  }
};

class DeclName final {
  DeclNameKind kind = DeclNameKind::Basic;
  llvm::PointerUnion<DeclNameBase, CompoundDeclName *> nameUnion;

public:
  DeclName() = default;
  explicit DeclName(Identifier ident)
      : kind(DeclNameKind::Basic), nameUnion(DeclNameBase(ident)) {}

  DeclName(DeclNameKind kind, DeclNameBase base)
      : kind(kind), nameUnion(base) {}

  DeclName(CompoundDeclName *compound)
      : kind(DeclNameKind::Compound), nameUnion(compound) {}

  DeclNameKind GetKind() const { return kind; }
  bool IsBasic() const { return kind == DeclNameKind::Basic; }
  bool IsSpecial() const { return !IsBasic(); }

  DeclNameBase GetBase() const {
    if (auto *compound = nameUnion.dyn_cast<CompoundDeclName *>()) {
      return compound->nameBase;
    }
    return nameUnion.get<DeclNameBase>();
  }

  Identifier GetIdentifier() const { return GetBase().GetIdentifier(); }

  // Compare for ordering or equality
  int Compare(const DeclName &other) const {
    auto lhsPtr = GetBase().GetIdentifier().GetPointer();
    auto rhsPtr = other.GetBase().GetIdentifier().GetPointer();
    if (lhsPtr < rhsPtr)
      return -1;
    if (lhsPtr > rhsPtr)
      return 1;
    return 0;
  }
};
} // namespace stone
// === LLVM Integration ===
namespace llvm {

template <> struct DenseMapInfo<stone::DeclNameBase> {
  static stone::DeclNameBase getEmptyKey() { return stone::DeclNameBase(); }
  static stone::DeclNameBase getTombstoneKey() { return stone::DeclNameBase(); }
  static unsigned getHashValue(stone::DeclNameBase val) {
    return DenseMapInfo<const void *>::getHashValue(val.GetAsOpaquePointer());
  }
  static bool isEqual(stone::DeclNameBase lhs, stone::DeclNameBase rhs) {
    return lhs == rhs;
  }
};

template <> struct PointerLikeTypeTraits<stone::DeclNameBase> {
  static inline void *getAsVoidPointer(stone::DeclNameBase d) {
    return const_cast<void *>(d.GetAsOpaquePointer());
  }
  static inline stone::DeclNameBase getFromVoidPointer(void *p) {
    return stone::DeclNameBase::GetFromOpaquePointer(p);
  }
  enum {
    NumLowBitsAvailable = PointerLikeTypeTraits<void *>::NumLowBitsAvailable
  };
};

} // namespace llvm

#endif // STONE_AST_DECLNAME_H
