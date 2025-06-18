#ifndef STONE_AST_TYPE_H
#define STONE_AST_TYPE_H

#include "stone/AST/InlineBitfield.h"
#include "stone/AST/Tree.h"
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

class alignas(1 << TypeAlignInBits) Type : public Tree {
  TypeState *TS = nullptr;

public:
  Type(TypeState *TS) : Tree(nullptr), TS(TS) {
    assert(TS && "Expected TypeState for a Type!");
    TS->SetOwner(this);
  }

public:
  virtual TypeKind GetKind() const { return TS->GetKind(); }
  TypeState *GetTypeState() { return TS; }
  ArtifactKind GetArtifactKind() const override { return ArtifactKind::Type; }

  bool IsCanType() const {
    return (TS->HasCanType() && TS->GetCanType() == this);
  }

  static bool classof(const Type *ty);
  static bool classof(const Tree *node);
};

class ObjectType : public Type {
public:
  ObjectType(TypeState *TS) : Type(TS) {}
};

class BuiltinType : public ObjectType {
public:
  BuiltinType(TypeState *TS) : ObjectType(TS) {}
};

class CharType final : public BuiltinType {
public:
  CharType(TypeState *TS) : BuiltinType(TS) {}
};

class BoolType final : public BuiltinType {
public:
  BoolType(TypeState *TS) : BuiltinType(TS) {}
};

class NullType final : public BuiltinType {
public:
  NullType(TypeState *TS) : BuiltinType(TS) {}
};

class NumberType : public BuiltinType {
public:
  NumberType(TypeState *TS) : BuiltinType(TS) {}
};

class IntType : public NumberType {
public:
  IntType(TypeState *TS) : NumberType(TS) {}
};

class Int8Type : public NumberType {
public:
  Int8Type(TypeState *TS) : NumberType(TS) {}
};

class Int16Type : public NumberType {
public:
  Int16Type(TypeState *TS) : NumberType(TS) {}
};

class Int32Type : public NumberType {
public:
  Int32Type(TypeState *TS) : NumberType(TS) {}
};

class Int64Type : public NumberType {
public:
  Int64Type(TypeState *TS) : NumberType(TS) {}
};

class Int128Type : public NumberType {
public:
  Int128Type(TypeState *TS) : NumberType(TS) {}
};

class UIntType : public NumberType {
public:
  UIntType(TypeState *TS) : NumberType(TS) {}
};

class UInt8Type : public NumberType {
public:
  UInt8Type(TypeState *TS) : NumberType(TS) {}
};

class UInt16Type : public NumberType {
public:
  UInt16Type(TypeState *TS) : NumberType(TS) {}
};

class UInt32Type : public NumberType {
public:
  UInt32Type(TypeState *TS) : NumberType(TS) {}
};

class UInt64Type : public NumberType {
public:
  UInt64Type(TypeState *TS) : NumberType(TS) {}
};

class UInt128Type : public NumberType {
public:
  UInt128Type(TypeState *TS) : NumberType(TS) {}
};

class FloatType : public NumberType {
public:
  FloatType(TypeState *TS) : NumberType(TS) {}
  const llvm::fltSemantics &GetAPFloatSemantics() const;
};

class Float16Type : public NumberType {
public:
  Float16Type(TypeState *TS) : NumberType(TS) {}
};

class Float32Type : public NumberType {
public:
  Float32Type(TypeState *TS) : NumberType(TS) {}
};

class Float64Type : public NumberType {
public:
  Float64Type(TypeState *TS) : NumberType(TS) {}
};

class Float128Type : public NumberType {
public:
  Float128Type(TypeState *TS) : NumberType(TS) {}
};

class NominalType : public ObjectType {
public:
  NominalType(TypeState *TS) : ObjectType(TS) {}
};

class StructType final : public NominalType {
public:
  StructType(TypeState *TS) : NominalType(TS) {}
};

class InterfaceType final : public NominalType {
public:
  InterfaceType(TypeState *TS) : NominalType(TS) {}
};

class EnumType final : public NominalType {
public:
  EnumType(TypeState *TS) : NominalType(TS) {}
};

class SugType : public ObjectType {
public:
  SugType(TypeState *TS) : ObjectType(TS) {}
};

class AliasType final : public SugType,
                        public llvm::FoldingSetNode,
                        llvm::TrailingObjects<AliasType, Type> {
  // In TypeState AliasDecl *aliasDecl;

public:
  AliasType(TypeState *TS) : SugType(TS) {}
};

class StringType final : public SugType {
public:
  StringType(TypeState *TS) : SugType(TS) {}
};

class DeducedType : public Type {
public:
  DeducedType(TypeState *TS) : Type(TS) {}
};

class AutoType final : public DeducedType {
public:
  AutoType(TypeState *TS) : DeducedType(TS) {}
};

class AccessType : public Type {
public:
  AccessType(TypeState *TS) : Type(TS) {}
};

class PointerType : public AccessType {
public:
  PointerType(TypeState *TS) : AccessType(TS) {}
};

class PtrType final : public PointerType {
public:
  PtrType(TypeState *TS) : PointerType(TS) {}
};

class MoveType final : public PointerType {
public:
  MoveType(TypeState *TS) : PointerType(TS) {}
};

class OwnType final : public PointerType {
public:
  OwnType(TypeState *TS) : PointerType(TS) {}
};

class SafeType final : public PointerType {
public:
  SafeType(TypeState *TS) : PointerType(TS) {}
};

class ReferenceType : public AccessType {
public:
  ReferenceType(TypeState *TS) : AccessType(TS) {}
};

class RefType final : public ReferenceType {
public:
  RefType(TypeState *TS) : ReferenceType(TS) {}
};

} // namespace stone

#endif
