#include "stone/AST/Type.h"
#include "stone/AST/ASTSession.h"
#include "stone/AST/TypeState.h"

using namespace stone;

ASTSession &Type::GetASTSession(TypeState *typeState) {
  assert(typeState && "Cannot get session from null TypeState");
  return typeState->GetASTSession();
}

BuiltinType::BuiltinType(TypeKind kind, TypeState *TS) : AnyType(kind, TS) {
  // Bits.Type.IsBuiltin = true;
  TS->SetCanonicalType(this);
}

NumberBitWidth NumberType::GetBitWidth() const {
  switch (GetKind()) {
  case TypeKind::Int:
  case TypeKind::UInt:
  case TypeKind::Float:
    return NumberBitWidth::Platform;
  case TypeKind::Int8:
    return NumberBitWidth::Size8;
  case TypeKind::Int16:
    return NumberBitWidth::Size16;
  case TypeKind::Int32:
    return NumberBitWidth::Size32;
  case TypeKind::Int64:
    return NumberBitWidth::Size64;
  case TypeKind::Int128:
    return NumberBitWidth::Size128;
  case TypeKind::UInt8:
    return NumberBitWidth::Size8;
  case TypeKind::UInt16:
    return NumberBitWidth::Size16;
  case TypeKind::UInt32:
    return NumberBitWidth::Size32;
  case TypeKind::UInt64:
    return NumberBitWidth::Size64;
  case TypeKind::UInt128:
    return NumberBitWidth::Size128;
  case TypeKind::Float16:
    return NumberBitWidth::Size16;
  case TypeKind::Float32:
    return NumberBitWidth::Size32;
  case TypeKind::Float64:
    return NumberBitWidth::Size64;
  case TypeKind::Float128:
    return NumberBitWidth::Size128;
  }
  llvm_unreachable("Unknown number type");
}
