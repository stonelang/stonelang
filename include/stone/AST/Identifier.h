#ifndef STONE_AST_IDENTIFIER_H
#define STONE_AST_IDENTIFIER_H

#include "stone/Support/EditorPlaceholder.h"

#include "llvm/ADT/ArrayRef.h"
#include "llvm/ADT/DenseMapInfo.h"
#include "llvm/ADT/Hashing.h"
#include "llvm/ADT/SmallVector.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/ADT/StringSet.h"
#include "llvm/Support/Allocator.h"

namespace stone {

using IdentifierTable = llvm::StringSet<llvm::BumpPtrAllocator &>;
class Identifier final {
  const char *ptr;

public:
  Identifier() : ptr(nullptr) {}
  explicit Identifier(const char *str) : ptr(str) {}
  explicit operator bool() const { return !IsEmpty(); }
  bool operator==(Identifier other) const { return ptr == other.ptr; }
  bool operator!=(Identifier other) const { return ptr != other.ptr; }

public:
  const char *GetPtr() const { return ptr; }
  llvm::StringRef GetString() const {
    return !IsEmpty() ? llvm::StringRef(ptr) : llvm::StringRef();
  }
  bool IsEqual(llvm::StringRef other) const {
    if (IsEmpty()) {
      return false;
    }
    return GetString().equals(other);
  }
  bool IsEmpty() const { return ptr == nullptr; }
  bool HasTildaPrefix() const { return GetString().starts_with("~"); }
  bool IsArithmeticOperator() const;
  bool IsSTDComparisonOperator() const;
  bool IsEditorPlaceholder() const {
    return !IsEmpty() && isEditorPlaceholder(GetString());
  }

public:
  static bool IsOperatorStartCodePoint(uint32_t C);
  static bool IsOperatorContinuationCodePoint(uint32_t C);
  static Identifier FromString(llvm::StringRef str, IdentifierTable &table);

  static bool IsEditorPlaceholder(llvm::StringRef name) {
    return stone::isEditorPlaceholder(name);
  }
};

class DotName {
  llvm::SmallVector<Identifier, 4> segments;

public:
  DotName() = default;
  explicit DotName(llvm::ArrayRef<Identifier> segs)
      : segments(segs.begin(), segs.end()) {}

  void Add(Identifier id) { segments.push_back(id); }
  bool Empty() const { return segments.empty(); }
  size_t Size() const { return segments.size(); }

  Identifier GetFront() const { return segments.front(); }
  Identifier GetBack() const { return segments.back(); }
  llvm::ArrayRef<Identifier> GetSegments() const { return segments; }

  bool operator==(const DotName &other) const {
    return segments == other.segments;
  }
  bool operator!=(const DotName &other) const { return !(*this == other); }
};

} // namespace stone

namespace llvm {

template <> struct DenseMapInfo<stone::Identifier> {
  static stone::Identifier getEmptyKey() {
    return stone::Identifier(
        reinterpret_cast<const char *>(~static_cast<uintptr_t>(0)));
  }

  static stone::Identifier getTombstoneKey() {
    return stone::Identifier(
        reinterpret_cast<const char *>(~static_cast<uintptr_t>(1)));
  }

  static unsigned getHashValue(stone::Identifier Val) {
    return DenseMapInfo<const char *>::getHashValue(Val.GetPtr());
  }

  static bool isEqual(stone::Identifier LHS, stone::Identifier RHS) {
    return LHS == RHS;
  }
};

template <> struct DenseMapInfo<stone::DotName> {
  static stone::DotName getEmptyKey() {
    return stone::DotName({DenseMapInfo<stone::Identifier>::getEmptyKey()});
  }

  static stone::DotName getTombstoneKey() {
    return stone::DotName({DenseMapInfo<stone::Identifier>::getTombstoneKey()});
  }

  static unsigned getHashValue(const stone::DotName &dn) {
    unsigned hash = 0;
    for (auto id : dn.GetSegments()) {
      hash = llvm::hash_combine(
          hash, DenseMapInfo<stone::Identifier>::getHashValue(id));
    }
    return hash;
  }

  static bool isEqual(const stone::DotName &LHS, const stone::DotName &RHS) {
    return LHS == RHS;
  }
};

} // namespace llvm
#endif
