#include "stone/AST/TypeFlight.h"

using namespace stone;



// bool BuiltinTypeFlight::IsNumberType(TypeKind kind) const {
//   switch (kind) {
//   case TypeKind::Int:
//   case TypeKind::UInt:
//   case TypeKind::Float:
//   case TypeKind::Int8:
//   case TypeKind::Int16:
//   case TypeKind::Int32:
//   case TypeKind::Int64:
//   case TypeKind::Int128:
//   case TypeKind::UInt8:
//   case TypeKind::UInt16:
//   case TypeKind::UInt32:
//   case TypeKind::UInt64:
//   case TypeKind::UInt128:
//   case TypeKind::Float16:
//   case TypeKind::Float32:
//   case TypeKind::Float64:
//   case TypeKind::Float128:
//     return true;
//   default:
//     return false;
//   }
// }

// bool BuiltinTypeFlight::IsNumberType() const {
//   return BuiltinTypeFlight::IsNumberType(GetKind());
// }

// NumberBitWidth BuiltinTypeFlight::GetNumberBitWidth(TypeKind kind) {
//   switch (kind) {
//   case TypeKind::Int:
//   case TypeKind::UInt:
//   case TypeKind::Float:
//     return NumberBitWidth::Local;
//   case TypeKind::Int8:
//     return NumberBitWidth::Size8;
//   case TypeKind::Int16:
//     return NumberBitWidth::Size16;
//   case TypeKind::Int32:
//     return NumberBitWidth::Size32;
//   case TypeKind::Int64:
//     return NumberBitWidth::Size64;
//   case TypeKind::Int128:
//     return NumberBitWidth::Size128;
//   case TypeKind::UInt8:
//     return NumberBitWidth::Size8;
//   case TypeKind::UInt16:
//     return NumberBitWidth::Size16;
//   case TypeKind::UInt32:
//     return NumberBitWidth::Size32;
//   case TypeKind::UInt64:
//     return NumberBitWidth::Size64;
//   case TypeKind::UInt128:
//     return NumberBitWidth::Size128;
//   case TypeKind::Float16:
//     return NumberBitWidth::Size16;
//   case TypeKind::Float32:
//     return NumberBitWidth::Size32;
//   case TypeKind::Float64:
//     return NumberBitWidth::Size64;
//   case TypeKind::Float128:
//     return NumberBitWidth::Size128;
//   }
//   llvm_unreachable("Unknown number type");
// }

// NumberBitWidth BuiltinTypeFlight::GetNumberBitWidth() {
//   return BuiltinTypeFlight::GetNumberBitWidth(GetKind());
// }
