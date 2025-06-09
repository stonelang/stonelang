#ifndef STONE_AST_TYPE_H
#define STONE_AST_TYPE_H

#include "stone/AST/AST.h"
#include "stone/AST/TypeAlignment.h"

#include "llvm/Support/Casting.h"

namespace stone {

class Type;

class alignas(1 << TypeAlignInBits) Type : public ASTAllocation<Type> {
  friend class ASTContext;

public:
};

class BuiltinType : public Type {
public:
};

class NumberType : public BuiltinType {
public:
};

class SugType : public Type {
public:
};

class StringType final : public SugType {
public:
  /// Uint8Type* -- sugar for ptr uint8
  /// StringType(TypeState *TS) : BuiltinType(TypeKind::String, TS) {}
};

/// PtrType, MoveType, SafeType, OwnType
} // namespace stone
#endif