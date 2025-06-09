#include "stone/AST/Type.h"
#include "stone/AST/TypeState.h"

using namespace stone;

NumberBitWidth NumberType::GetBitWidth() const {
  switch (GetKind()) {
    // case TypeKind::Int:
    // case TypeKind::UInt:
    // case TypeKind::Float:
    //   return BitWidth::Size;
    // case TypeKind::Int8:
    //   return BitWidth::Size8;
    // case TypeKind::Int16:
    //   return BitWidth::Size16;
  case TypeKind::Int32:
    return NumberBitWidth::Size32;
    // case TypeKind::Int64:
    //   return BitWidth::Size64;
    // case TypeKind::Int128:
    //   return BitWidth::Size128;
    // case TypeKind::UInt8:
    //   return BitWidth::Size8;
    // case TypeKind::UInt16:
    //   return BitWidth::Size16;
    // case TypeKind::UInt32:
    //   return BitWidth::Size32;
    // case TypeKind::UInt64:
    //   return BitWidth::Size64;
    // case TypeKind::UInt128:
    //   return BitWidth::Size128;
    // case TypeKind::Float16:
    //   return BitWidth::Size16;
    // case TypeKind::Float32:
    //   return BitWidth::Size32;
    // case TypeKind::Float64:
    //   return BitWidth::Size64;
    // case TypeKind::Float128:
    //   return BitWidth::Size128;
  }
  llvm_unreachable("Unknown number type");
}
