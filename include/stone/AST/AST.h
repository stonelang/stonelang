#ifndef STONE_AST_AST_H
#define STONE_AST_AST_H

#include "llvm/ADT/PointerUnion.h"

namespace stone {
class SrcLoc;
class SrcRange;
class Expr;
class Stmt;
class Decl;
class Type;
class ASTWalker;
class ASTContext;

enum class DeclKind : uint8_t;
enum class ExprKind : uint8_t;
enum class StmtKind : uint8_t;

struct ASTNode final : public llvm::PointerUnion<Decl *, Expr *, Stmt *> {
  // Inherit the constructors from PointerUnion.
  using PointerUnion::PointerUnion;

  SrcRange GetSrcRange() const;

  /// Return the location of the start of the statement.
  SrcLoc GetStartLoc() const;

  /// Return the location of the end of the statement.
  SrcLoc GetEndLoc() const;

  void Walk(ASTWalker &walker);
  void Walk(ASTWalker &&walker) { Walk(walker); }
};

void *AllocateInASTContext(size_t bytes, const ASTContext &astContext,
                           unsigned alignment);

/// Types inheriting from this class are intended to be allocated in an
/// \c ASTContext allocator; you cannot allocate them by using a normal \c
/// new, and instead you must either provide an \c ASTContext or use a placement
/// \c new.
///
/// The template parameter is a type with the desired alignment. It is usually,
/// but not always, the type that is inheriting \c ASTAllocated.
template <typename AlignTy> class ASTAllocation {
public:
  // Make vanilla new/delete illegal.
  void *operator new(size_t bytes) throw() = delete;
  void operator delete(void *data) throw() = delete;

  // Only allow allocation using the allocator in MemoryContext
  // or by doing a placement new.
  void *operator new(size_t bytes, const ASTContext &ctx,
                     unsigned alignment = alignof(AlignTy)) {
    return stone::AllocateInASTContext(bytes, ctx, alignment);
  }
  void *operator new(size_t bytes, void *mem) throw() {
    assert(mem && "placement new into failed allocation");
    return mem;
  }
};

} // namespace stone
#endif
