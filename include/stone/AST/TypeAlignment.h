#ifndef STONE_AST_TYPEALIGNMENT_H
#define STONE_AST_TYPEALIGNMENT_H

#include <cstddef>

namespace stone {

class Decl;
class Expr;
class Stmt;
class Type;
class ASTSession;
class DeclContext;

constexpr size_t DeclAlignInBits = 3;
constexpr size_t ExprAlignInBits = 3;
constexpr size_t StmtAlignInBits = 3;
constexpr size_t TypeAlignInBits = 3;

constexpr size_t ASTSessionAlignInBits = 3;
constexpr size_t DeclContextAlignInBits = 3;

} // namespace stone

namespace llvm {
/// Helper class for declaring the expected alignment of a pointer.
/// TODO: LLVM should provide this.
template <class T, size_t AlignInBits> struct MoreAlignedPointerTraits {
  enum { NumLowBitsAvailable = AlignInBits };
  static inline void *getAsVoidPointer(T *ptr) { return ptr; }
  static inline T *getFromVoidPointer(void *ptr) {
    return static_cast<T *>(ptr);
  }
};

template <class T> struct PointerLikeTypeTraits;
} // namespace llvm

/// Declare the expected alignment of pointers to the given type.
/// This macro should be invoked from a top-level file context.
#define LLVM_DECLARE_TYPE_ALIGNMENT(CLASS, ALIGNMENT)                          \
  namespace llvm {                                                             \
  template <>                                                                  \
  struct PointerLikeTypeTraits<CLASS *>                                        \
      : public MoreAlignedPointerTraits<CLASS, ALIGNMENT> {};                  \
  }

LLVM_DECLARE_TYPE_ALIGNMENT(stone::Decl, stone::DeclAlignInBits)
LLVM_DECLARE_TYPE_ALIGNMENT(stone::Expr, stone::ExprAlignInBits)
LLVM_DECLARE_TYPE_ALIGNMENT(stone::Stmt, stone::StmtAlignInBits)
LLVM_DECLARE_TYPE_ALIGNMENT(stone::Type, stone::TypeAlignInBits)

static_assert(alignof(void *) >= 2, "pointer alignment is too small");

#endif
