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

} // namespace stone
#endif
