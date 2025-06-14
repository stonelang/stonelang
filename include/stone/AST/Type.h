#ifndef STONE_AST_TYPE_H
#define STONE_AST_TYPE_H

#include "stone/AST/ASTUnit.h"
#include "stone/AST/InlineBitfield.h"
#include "stone/AST/TypeAlignment.h"

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
class ASTSession;
class AliasDecl;

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

class alignas(1 << TypeAlignInBits) Type : public ASTUnit {
  friend ASTSession;
  TypeKind kind;
  TypeState *typeState = nullptr;

  ASTSession &GetASTSession(TypeState *typeState);

public:
  Type(TypeKind kind, TypeState *typeState)
      : ASTUnit(GetASTSession(typeState)), kind(kind), typeState(typeState) {}

public:
  TypeKind GetKind() const { return kind; }
  TypeState *GetTypeState();
  ASTUnitKind GetUnitKind() const override { return ASTUnitKind::Type; }

public:
  static bool classof(const ASTUnit *unit) {
    return unit->GetUnitKind() == ASTUnitKind::Type;
  }
};

class AnyType : public Type {
public:
  AnyType(TypeKind kind, TypeState *typeState) : Type(kind, typeState) {}
};

class BuiltinType : public AnyType {
public:
  explicit BuiltinType(TypeKind kind, TypeState *TS);
};

class CharType final : public BuiltinType {
public:
  CharType(TypeState *TS) : BuiltinType(TypeKind::Char, TS) {}
};

class BoolType final : public BuiltinType {
public:
  BoolType(TypeState *TS) : BuiltinType(TypeKind::Bool, TS) {}
};

class NullType final : public BuiltinType {
public:
  NullType(TypeState *TS) : BuiltinType(TypeKind::Null, TS) {}
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
  friend ASTSession;
  NumberBitWidth numberBitWidth;

  bool IsNumberType(TypeKind kind, TypeState *typeState) const;

public:
  NumberType(TypeKind kind, TypeState *typeState)
      : BuiltinType(kind, typeState) {}

public:
  NumberBitWidth GetBitWidth() const;
};
class IntType : public NumberType {
  friend ASTSession;

public:
  IntType(TypeState *TS) : NumberType(TypeKind::Int, TS) {}

public:
  static IntType *Create(const ASTSession &AC);
};

class Int8Type : public NumberType {
  friend ASTSession;

public:
  Int8Type(TypeState *TS) : NumberType(TypeKind::Int8, TS) {}
};

class Int16Type : public NumberType {
  friend ASTSession;

public:
  Int16Type(TypeState *TS) : NumberType(TypeKind::Int16, TS) {}
};

class Int32Type : public NumberType {
  friend ASTSession;

public:
  Int32Type(TypeState *TS) : NumberType(TypeKind::Int32, TS) {}
};

class Int64Type : public NumberType {
  friend ASTSession;

public:
  Int64Type(TypeState *TS) : NumberType(TypeKind::Int64, TS) {}
};

class Int128Type : public NumberType {
  friend ASTSession;

public:
  Int128Type(TypeState *TS) : NumberType(TypeKind::Int128, TS) {}
};

class UIntType : public NumberType {
  friend class ASTSession;

public:
  UIntType(TypeState *TS) : NumberType(TypeKind::UInt, TS) {}
};
class UInt8Type : public NumberType {
  friend class ASTSession;

public:
  UInt8Type(TypeState *TS) : NumberType(TypeKind::UInt8, TS) {}
};
class UInt16Type : public NumberType {
  friend class ASTSession;

public:
  UInt16Type(TypeState *TS) : NumberType(TypeKind::UInt16, TS) {}
};

class UInt32Type : public NumberType {
  friend class ASTSession;

public:
  UInt32Type(TypeState *TS) : NumberType(TypeKind::UInt32, TS) {}
};

class UInt64Type final : public NumberType {
  friend class ASTSession;

public:
  UInt64Type(TypeState *TS) : NumberType(TypeKind::UInt64, TS) {}
};
class UInt128Type final : public NumberType {
  friend class ASTSession;

public:
  UInt128Type(TypeState *TS) : NumberType(TypeKind::UInt128, TS) {}
};

class FloatType : public NumberType {
  friend ASTSession;

public:
  FloatType(TypeState *TS) : NumberType(TypeKind::Float, TS) {}

public:
  const llvm::fltSemantics &GetAPFloatSemantics() const;

public:
  static FloatType *Create(const ASTSession &astContext);

  static bool classof(const Type *T) { return T->GetKind() == TypeKind::Float; }
};

class Float16Type : public NumberType {
  friend ASTSession;

public:
  Float16Type(TypeState *TS) : NumberType(TypeKind::Float16, TS) {}
};

class Float32Type : public NumberType {
  friend ASTSession;

public:
  Float32Type(TypeState *TS) : NumberType(TypeKind::Float32, TS) {}
};

class Float64Type : public NumberType {
  friend ASTSession;

public:
  Float64Type(TypeState *TS) : NumberType(TypeKind::Float64, TS) {}
};
class Float128Type : public NumberType {
  friend ASTSession;

public:
  Float128Type(TypeState *TS) : NumberType(TypeKind::Float128, TS) {}
};

class NominalType : public AnyType {
public:
  NominalType(TypeKind kind, TypeState *TS) : AnyType(kind, TS) {}
};

class StructType final : public NominalType {
public:
  StructType(TypeState *TS) : NominalType(TypeKind::Struct, TS) {}
};

class InterfaceType final : public NominalType {
public:
  InterfaceType(TypeState *TS) : NominalType(TypeKind::Interface, TS) {}
};

class EnumType final : public NominalType {

public:
  EnumType(TypeState *TS) : NominalType(TypeKind::Enum, TS) {}
};

class SugType : public AnyType {
public:
  SugType(TypeKind kind, TypeState *TS) : AnyType(TypeKind::Enum, TS) {}
};

/// A reference to a type alias that is somehow generic, along with the
/// set of substitutions to apply to make the type concrete.
class AliasType final : public SugType,
                        public llvm::FoldingSetNode,
                        llvm::TrailingObjects<AliasType, Type> {
  AliasDecl *aliasDecl;

public:
  AliasType(TypeState *TS) : SugType(TypeKind::Alias, TS) {}
};

class StringType final : public SugType {
public:
  /// Uint8Type* -- sugar for ptr uint8
  /// StringType(TypeState *TS) : BuiltinType(TypeKind::String, TS) {}
};

class DeducedType : public Type {
protected:
  friend class ASTSession; // ASTSession creates these
};
// own auto in = new int
class AutoType final : public DeducedType, public llvm::FoldingSetNode {
public:
};

class AccessType : public Type {
public:
  AccessType(TypeKind kind, TypeState *TS) : Type(kind, TS) {}
};

// === Pointer Types ===
class PointerType : public AccessType {
public:
  PointerType(TypeKind kind, TypeState *TS) : AccessType(kind, TS) {}
};

class PtrType final : public PointerType {
public:
  PtrType(TypeState *TS) : PointerType(TypeKind::Ptr, TS) {}
};

class MoveType final : public PointerType {
public:
  MoveType(TypeState *TS) : PointerType(TypeKind::Move, TS) {}
};

class OwnType final : public PointerType {
public:
  OwnType(TypeState *TS) : PointerType(TypeKind::Own, TS) {}
};

class SafeType final : public PointerType {
public:
  SafeType(TypeState *TS) : PointerType(TypeKind::Safe, TS) {}
};

// === Reference Types ===
class ReferenceType : public AccessType {
public:
  ReferenceType(TypeKind kind, TypeState *TS) : AccessType(kind, TS) {}
};

class RefType final : public ReferenceType {
public:
  RefType(TypeState *TS) : ReferenceType(TypeKind::Ref, TS) {}
};

} // namespace stone
#endif