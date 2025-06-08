#ifndef STONE_AST_DECL_H
#define STONE_AST_DECL_H

#include "stone/AST/AST.h"
#include "stone/AST/InlineBitfield.h"
#include "stone/AST/TypeAlignment.h"
#include "llvm/Support/Casting.h"

#include <stdint.h>

namespace stone {

class DeclContext;
class ASTContext;
using DeclContextOrASTContext = llvm::PointerUnion<DeclContext *, ASTContext *>;

enum class DeclKind : uint8_t {
  None = 0,
#define DECL(Id, Parent) Id,
#define LAST_DECL(Id) Count = Id,
#define DECL_RANGE(Id, FirstId, LastId)                                        \
  First##Id##Decl = FirstId, Last##Id##Decl = LastId,
#include "stone/AST/DeclNode.def"
};

enum : uint8_t {
  NumDeclKindBits = stone::CountBitsUsed(static_cast<unsigned>(DeclKind::Count))
};

// Introduces a name and associates it with a type such as:
// int x where x is the declaration, int is the type.
class alignas(1 << DeclAlignInBits) Decl : public ASTAllocation<Decl> {
public:
};

class ValueDecl : public Decl {
public:
};

class TypeDecl : public ValueDecl {};

} // namespace stone
#endif
