#ifndef STONE_AST_DECLCONTEXT_H
#define STONE_AST_DECLCONTEXT_H

#include "stone/AST/AST.h"

#include "llvm/ADT/PointerUnion.h"
#include "llvm/Support/Casting.h"

namespace stone {
class Decl;
class Expr;
class Stmt;
class ModuleFile;

using DeclContextType =
    llvm::PointerUnion<Decl *, Expr *, Stmt *, ModuleFile *>;

class DeclContext {
  DeclContextType parentType;

public:
  DeclContext(DeclContextType parentType = nullptr);
};

} // namespace stone
#endif
