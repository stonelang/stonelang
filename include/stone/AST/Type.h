#ifndef STONE_AST_TYPE_H
#define STONE_AST_TYPE_H

#include "stone/AST/InlineBitfield.h"
#include "stone/AST/Node.h"
#include "stone/AST/TypeAlignment.h"
#include "stone/AST/TypeKind.h"

#include "llvm/ADT/APFloat.h"
#include "llvm/ADT/APInt.h"
#include "llvm/ADT/APSInt.h"
#include "llvm/ADT/ArrayRef.h"
#include "llvm/ADT/FoldingSet.h"
#include "llvm/Support/Casting.h"
#include "llvm/Support/TrailingObjects.h"

namespace stone {

class Type;
class TypeState;

class alignas(1 << TypeAlignInBits) Type : public Node {
  TypeState *TS = nullptr;

public:
  Type(TypeState *TS);

public:
  TypeKind GetKind() const;
  TypeState *GetTypeState() { return TS; }
  bool IsCanType() const;
  ArtifactKind GetArtifactKind() const override { return ArtifactKind::Type; }

public:
  static bool classof(const Type *ty);
  static bool classof(const Node *node);
};

// === Function Type ===
// class FunType final : public Type {
// public:
//   explicit FunType(TypeState *TS) : Type(TS) {}
// };

// // === Object Types (boxable, runtime types) ===
class ObjectType : public Type {
public:
  explicit ObjectType(TypeState *TS) : Type(TS) {}
};

// // === Nominal Types ===
// class NominalType : public ObjectType {
// public:
//   explicit NominalType(TypeState *TS) : ObjectType(TS) {}
// };

// class EnumType final : public NominalType {
// public:
//   explicit EnumType(TypeState *TS) : NominalType(TS) {}
// };

// class StructType final : public NominalType {
// public:
//   explicit StructType(TypeState *TS) : NominalType(TS) {}

// public:
// };

// class InterfaceType final : public NominalType {
// public:
//   explicit InterfaceType(TypeState *TS) : NominalType(TS) {}

// public:
// };

// // === Builtin Types ===
class BuiltinType : public ObjectType {
public:
  explicit BuiltinType(TypeState *TS);
};

// class NumberType : public BuiltinType {
// public:
//   explicit NumberType(TypeState *TS) : BuiltinType(TS) {}
// };

// // Integers
// class IntType final : public NumberType {
// public:
//   explicit IntType(TypeState *TS) : NumberType(TS) {}
// };
// class Int8Type final : public NumberType {
// public:
//   explicit Int8Type(TypeState *TS) : NumberType(TS) {}
// };
// class Int16Type final : public NumberType {
// public:
//   explicit Int16Type(TypeState *TS) : NumberType(TS) {}
// };
// class Int32Type final : public NumberType {
// public:
//   explicit Int32Type(TypeState *TS) : NumberType(TS) {}
// };
// class Int64Type final : public NumberType {
// public:
//   explicit Int64Type(TypeState *TS) : NumberType(TS) {}
// };
// class Int128Type final : public NumberType {
// public:
//   explicit Int128Type(TypeState *TS) : NumberType(TS) {}
// };

// // Unsigned integers
// class UIntType final : public NumberType {
// public:
//   explicit UIntType(TypeState *TS) : NumberType(TS) {}
// };
// class UInt8Type final : public NumberType {
// public:
//   explicit UInt8Type(TypeState *TS) : NumberType(TS) {}
// };
// class UInt16Type final : public NumberType {
// public:
//   explicit UInt16Type(TypeState *TS) : NumberType(TS) {}
// };
// class UInt32Type final : public NumberType {
// public:
//   explicit UInt32Type(TypeState *TS) : NumberType(TS) {}
// };
// class UInt64Type final : public NumberType {
// public:
//   explicit UInt64Type(TypeState *TS) : NumberType(TS) {}
// };
// class UInt128Type final : public NumberType {
// public:
//   explicit UInt128Type(TypeState *TS) : NumberType(TS) {}
// };

// // Floating point
// class FloatType final : public NumberType {
// public:
//   explicit FloatType(TypeState *TS) : NumberType(TS) {}
// };
// class Float16Type final : public NumberType {
// public:
//   explicit Float16Type(TypeState *TS) : NumberType(TS) {}
// };
// class Float32Type final : public NumberType {
// public:
//   explicit Float32Type(TypeState *TS) : NumberType(TS) {}
// };
// class Float64Type final : public NumberType {
// public:
//   explicit Float64Type(TypeState *TS) : NumberType(TS) {}
// };
// class Float128Type final : public NumberType {
// public:
//   explicit Float128Type(TypeState *TS) : NumberType(TS) {}
// };

// // Other builtins
// class BoolType final : public BuiltinType {
// public:
//   explicit BoolType(TypeState *TS) : BuiltinType(TS) {}
// };

// class CharType final : public BuiltinType {
// public:
//   explicit CharType(TypeState *TS) : BuiltinType(TS) {}
// };

// class NullType final : public BuiltinType {
// public:
//   explicit NullType(TypeState *TS) : BuiltinType(TS) {}
// };

// class StringType final : public BuiltinType {
// public:
//   explicit StringType(TypeState *TS) : BuiltinType(TS) {}
// };

// // === Magic (non-canonical) Types ===
// class MagicType : public Type {
// public:
//   explicit MagicType(TypeState *TS) : Type(TS) {}
// };

// class AliasType final : public MagicType {
// public:
//   explicit AliasType(TypeState *TS) : MagicType(TS) {}
// };

// class AutoType final : public MagicType {
// public:
//   explicit AutoType(TypeState *TS) : MagicType(TS) {}
// };

// // === Access Types (non-boxable) ===
// class AccessType : public Type {
// public:
//   explicit AccessType(TypeState *TS) : Type(TS) {}
// };

// class PtrType final : public AccessType {
// public:
//   explicit PtrType(TypeState *TS) : AccessType(TS) {}
// };

// class RefType final : public AccessType {
// public:
//   explicit RefType(TypeState *TS) : AccessType(TS) {}
// };

// // === Aggregate Types (structural composites) ===
// class AggregateType : public ObjectType {
// public:
//   explicit AggregateType(TypeState *TS) : ObjectType(TS) {}
// };

// class ArrayType final : public AggregateType {
// public:
//   explicit ArrayType(TypeState *TS) : AggregateType(TS) {}
// };

// class TupleType final : public AggregateType {
// public:
//   explicit TupleType(TypeState *TS) : AggregateType(TS) {}
// };

// class VariadicType final : public AggregateType {
// public:
//   explicit VariadicType(TypeState *TS) : AggregateType(TS) {}
// };

} // namespace stone

#endif
