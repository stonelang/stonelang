#ifndef STONE_AST_TYPE_H
#define STONE_AST_TYPE_H

#include "stone/AST/AST.h"
#include "stone/AST/TypeAlignment.h"

#include "llvm/Support/Casting.h"

namespace stone {

class Type;

class alignas(1 << TypeAlignInBits) Type
    : public ASTAllocation<std::aligned_storage<8, 8>::type> {
  friend class ASTContext;

public:
};

class PrimitiveType : public Type {
public:
};

class BuiltinType : public PrimitiveType {
public:
};

class NumberType : public BuiltinType {
public:
};

} // namespace stone
#endif