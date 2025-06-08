#ifndef STONE_AST_TYPE_H
#define STONE_AST_TYPE_H

#include <cstdint>
#include <cstring>
#include <optional>
#include <string>
#include <type_traits>
#include <utility>

#include "llvm/Support/Casting.h"

namespace stone {

class Type;

class Type {
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