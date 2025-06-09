#ifndef STONE_AST_TYPE_H
#define STONE_AST_TYPE_H

#include "stone/AST/AST.h"
#include "stone/AST/TypeAlignment.h"
#include "stone/AST/InlineBitfield.h"

#include "llvm/Support/Casting.h"

namespace stone {

class Type;
class TypeState;
class ASTContext;

enum class TypeKind : uint8_t {
  None = 0,
#define TYPE(id, parent) id,
#define LAST_TYPE(id) Last_Type = id,
#define TYPE_RANGE(Id, FirstId, LastId)                                        \
  First_##Id##Type = FirstId, Last_##Id##Type = LastId,
#include "stone/AST/TypeNode.def"
};

enum : uint8_t {
  NumTypeKindBits =
      stone::CountBitsUsed(static_cast<unsigned>(TypeKind::Last_Type))
};

class alignas(1 << TypeAlignInBits) Type : public ASTAllocation<Type> {
  friend ASTContext;
  TypeKind kind;
  TypeState *typeState = nullptr;

public:
  Type(TypeKind kind, TypeState *typeState)
      : kind(kind), typeState(typeState) {}

public:
  TypeKind GetKind() const { return kind; }
  TypeState *GetTypeState();
};

class BuiltinType : public Type {
public:
  BuiltinType(TypeKind kind, TypeState *typeState) : Type(kind, typeState) {}
};

enum class NumberBitWidth : uint8_t {
  Platform, // Platform-dependent
  Size8,    // 8 bits
  Size16,   // 16 bits
  Size32,   // 32 bits
  Size64,   // 64 bits
  Size128   // 128 bits
};
class NumberType : public BuiltinType {
  friend ASTContext;
  NumberBitWidth numberBitWidth;

  bool IsNumberType(TypeKind kind, TypeState *typeState) const;

public:
  NumberType(TypeKind kind, TypeState *typeState)
      : BuiltinType(kind, typeState) {}

public:
  NumberBitWidth GetBitWidth() const;
};

class Int32Type final : public NumberType {
public:
  Int32Type(TypeState *typeState) : NumberType(TypeKind::Int32, typeState) {}
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