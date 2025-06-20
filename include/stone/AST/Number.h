#ifndef STONE_AST_NUMBER_H
#define STONE_AST_NUMBER_H

namespace stone {

enum class NumberBitWidth : uint8_t {
  Local = 0,
  Size8,
  Size16,
  Size32,
  Size64,
  Size128
};

} // namespace stone

#endif