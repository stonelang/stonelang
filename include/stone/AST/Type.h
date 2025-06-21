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
class FunType final : public Type {
public:
  explicit FunType(TypeState *TS) : Type(TS) {}
};

class VoidType : public Type {
public:
  explicit VoidType(TypeState *TS) : Type(TS) {}
};

// // === Nominal Types ===
class NominalType : public Type {
public:
  explicit NominalType(TypeState *TS) : Type(TS) {}
};

class EnumType final : public NominalType {
public:
  explicit EnumType(TypeState *TS) : NominalType(TS) {}
};

class StructType final : public NominalType {
public:
  explicit StructType(TypeState *TS) : NominalType(TS) {}

public:
};

class InterfaceType final : public NominalType {
public:
  explicit InterfaceType(TypeState *TS) : NominalType(TS) {}

public:
};

class IntType final : public Type {
public:
  explicit IntType(TypeState *TS) : Type(TS) {}
};

class UIntType final : public Type {
public:
  explicit UIntType(TypeState *TS) : Type(TS) {}
};

class FloatType final : public Type {
public:
  explicit FloatType(TypeState *TS) : Type(TS) {}
};

// Other builtins
class BoolType final : public Type {
public:
  explicit BoolType(TypeState *TS) : Type(TS) {}
};

class CharType final : public Type {
public:
  explicit CharType(TypeState *TS) : Type(TS) {}
};

class NullType final : public Type {
public:
  explicit NullType(TypeState *TS) : Type(TS) {}
};

class StringType final : public Type {
public:
  explicit StringType(TypeState *TS) : Type(TS) {}
};

// === Magic (non-canonical) Types ===
class MagicType : public Type {
public:
  explicit MagicType(TypeState *TS) : Type(TS) {}
};

class AliasType final : public MagicType {
public:
  explicit AliasType(TypeState *TS) : MagicType(TS) {}
};

class AutoType final : public MagicType {
public:
  explicit AutoType(TypeState *TS) : MagicType(TS) {}
};

// === Access Types (non-boxable) ===
class AccessType : public Type {
public:
  explicit AccessType(TypeState *TS) : Type(TS) {}
};

class PtrType final : public AccessType {
public:
  explicit PtrType(TypeState *TS) : AccessType(TS) {}
};

class RefType final : public AccessType {
public:
  explicit RefType(TypeState *TS) : AccessType(TS) {}
};

// === Aggregate Types (structural composites) ===
class AggregateType : public Type {
public:
  explicit AggregateType(TypeState *TS) : Type(TS) {}
};

class ArrayType final : public AggregateType {
public:
  explicit ArrayType(TypeState *TS) : AggregateType(TS) {}
};

class TupleType final : public AggregateType {
public:
  explicit TupleType(TypeState *TS) : AggregateType(TS) {}
};

class VariadicType final : public AggregateType {
public:
  explicit VariadicType(TypeState *TS) : AggregateType(TS) {}
};

} // namespace stone

#endif
