#ifndef STONE_AST_NODEKIND_H
#define STONE_AST_NODEKIND_H

#include <cstdint>
/// \file
/// \brief Defines `NodeKind`, a lightweight enum to classify high-level node categories.

namespace stone {
/// \brief Represents the broad classification of AST nodes.
///
/// `NodeKind` is used to categorize node entities (modules, declarations, types, etc.)
/// at a coarse level. This enables quick runtime checks, dispatching, and debugging
/// across the syntax tree without requiring full type introspection.
enum class NodeKind : uint8_t {
  None = 0,   ///< No node kind (used as default/uninitialized state)

  // === Module Hierarchy ===
  Module,     ///< Represents a top-level module in the AST

  // === Module File ===
  File,       ///< Represents a source file within a module

  // === Declarations ===
  Decl,       ///< Base kind for all declarations (functions, variables, types, etc.)

  // === Types ===
  Type,       ///< Base kind for all type nodes (e.g., NamedType, FunctionType)

  // === Expressions ===
  Expr,       ///< Base kind for all expression nodes (e.g., binary ops, calls)

  // === Statements ===
  Stmt,       ///< Base kind for all statement nodes (e.g., if, return, block)

  // === Meta: Used to compute bounds for kind dispatch ===
  Last = Stmt ///< Highest valid tag value (used for bounds checking or iteration)
};

} // namespace stone

#endif // STONE_AST_NODEKIND_H
