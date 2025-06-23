#ifndef STONE_AST_ALLOCATION_H
#define STONE_AST_ALLOCATION_H

#include "stone/AST/Memory.h"

#include "llvm/ADT/PointerUnion.h"

namespace stone {
/// Types inheriting from this class are intended to be allocated in an
/// \c Memory allocator; you cannot allocate them by using a normal \c
/// new, and instead you must either provide an \c Memory or use a
/// placement \c new.
///
/// The template parameter is a type with the desired alignment. It is usually,
/// but not always, the type that is inheriting \c ASTAllocated.
template <typename AlignTy> class Allocation {
public:
  // static_assert(alignof(AlignTy) >= alignof(void *),
  //               "Alignment must be at least pointer-aligned.");

  // Make vanilla new/delete illegal.
  void *operator new(size_t bytes) throw() = delete;
  void operator delete(void *data) throw() = delete;

  // Only allow allocation using the allocator in MemoryContext
  // or by doing a placement new.
  void *operator new(size_t bytes, const Memory &memory,
                     unsigned alignment = alignof(AlignTy)) {
    return memory.AllocateMemory(bytes, alignment);
  }
  void *operator new(size_t bytes, void *mem) throw() {
    assert(mem && "placement new into failed allocation");
    return mem;
  }
};

} // namespace stone
#endif
