#ifndef STONE_AST_ASTCONTEXT_H
#define STONE_AST_ASTCONTEXT_H

#include "stone/AST/Identifier.h"
#include "llvm/ADT/DenseMapInfo.h"
#include "llvm/ADT/FoldingSet.h"
#include "llvm/ADT/PointerIntPair.h"
#include "llvm/ADT/PointerUnion.h"
#include "llvm/ADT/SmallString.h"
#include "llvm/ADT/StringMap.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/Support/Allocator.h"
#include "llvm/Support/PointerLikeTypeTraits.h"
#include "llvm/Support/type_traits.h"
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <string>
#include <utility>

namespace stone {

class ASTContext final {
  mutable llvm::BumpPtrAllocator allocator;

  // FIXME: This is a StringMap rather than a StringSet because StringSet
  // doesn't allow passing in a pre-existing allocator.
  llvm::StringMap<Identifier *, llvm::BumpPtrAllocator &> identifiers;

public:
  Identifier &GetIdentifier(llvm::StringRef identStr);
  /// Retrieve the allocator for the given arena.
  llvm::BumpPtrAllocator &GetAllocator() const { return allocator; }

public:
  /// Allocate - Allocate memory from the Driver bump pointer.
  void *Allocate(unsigned long bytes, unsigned alignment = 8) const {
    if (bytes == 0) {
      return nullptr;
    }
    return GetAllocator().Allocate(bytes, alignment);
  }
};

} // namespace stone
#endif
