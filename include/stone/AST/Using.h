#ifndef STONE_AST_USING_H
#define STONE_AST_USING_H

#include "stone/AST/AST.h"
#include "stone/AST/Decl.h"
#include "stone/AST/DeclContext.h"

#include "llvm/ADT/SmallVector.h"

namespace stone {

class UsingPath {};

class UsingDecl : public ValueDecl {}

} // namespace stone
#endif