#ifndef STONE_AST_FOREIGN_H
#define STONE_AST_FOREIGN_H

#include <cstdint>

namespace stone {

enum class ForeignModuleDeclKind : uint8_t {
  Clang = 0, ///< Imported via Clang bridge
  DWARF      ///< Possibly from debug symbols
};

}

#endif




