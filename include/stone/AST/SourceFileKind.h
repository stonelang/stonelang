#ifndef STONE_AST_SOURCE_FILEKIND_H
#define STONE_AST_SOURCE_FILEKIND_H

#include <cstdint>

namespace stone {
enum class SourceFileKind : uint8_t {
  Normal = 0,     ///< A .stone file.
  Interface,      ///< A.stoneinterface file -- another module (stable ABI?)
  MacroExpansion, ///< Came from a macro expansion.
};
}

#endif