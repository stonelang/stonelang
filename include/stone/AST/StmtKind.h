#ifndef STONE_AST_STMT_KIND_H
#define STONE_AST_STMT_KIND_H

#include <cstdint>

namespace stone {

enum class StmtKind : uint8_t {
  None = 0,
  // Core block and control flow
  Brace,
  Return,
  Defer,
  Case,
  Break,
  Free,
  Continue,

  // Conditional statements
  If,
  Else,

  // Labeled control flow
  For,
  Match,

  // Exception handling
  Throw,
  Catch,
  Next,

  // Always last
  Count
};

// === Range markers for grouping ===

constexpr StmtKind FirstStmt = StmtKind::Brace;

// Conditional statements (LabeledConditionalStmt)
constexpr StmtKind FirstConditional = StmtKind::If;
constexpr StmtKind LastConditional = StmtKind::Else;

// Labeled statements (LabeledStmt)
constexpr StmtKind FirstLabeled = StmtKind::If;
constexpr StmtKind LastLabeled = StmtKind::Match;

constexpr StmtKind LastStmt = StmtKind::Next;

} // namespace stone

#endif // STONE_AST_STMT_KIND_H
