#ifndef STONE_AST_ASTALLOCATION_H
#define STONE_AST_ASTALLOCATION_H

#include "llvm/ADT/PointerUnion.h"

namespace stone {

class ASTUnit;
class ASTSession;
void *AllocateInASTSession(size_t bytes, const ASTSession &session,
                           unsigned alignment);
/// Types inheriting from this class are intended to be allocated in an
/// \c ASTSession allocator; you cannot allocate them by using a normal \c
/// new, and instead you must either provide an \c ASTSession or use a placement
/// \c new.
///
/// The template parameter is a type with the desired alignment. It is usually,
/// but not always, the type that is inheriting \c ASTAllocated.
template <typename AlignTy> class ASTAllocation {
public:
  static_assert(alignof(AlignTy) >= alignof(void *),
                "Alignment must be at least pointer-aligned.");

  // Make vanilla new/delete illegal.
  void *operator new(size_t bytes) throw() = delete;
  void operator delete(void *data) throw() = delete;

  // Only allow allocation using the allocator in MemoryContext
  // or by doing a placement new.
  void *operator new(size_t bytes, const ASTSession &session,
                     unsigned alignment = alignof(AlignTy)) {
    return stone::AllocateInASTSession(bytes, session, alignment);
  }
  void *operator new(size_t bytes, void *mem) throw() {
    assert(mem && "placement new into failed allocation");
    return mem;
  }
};

} // namespace stone
#endif
