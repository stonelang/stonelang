#ifndef STONE_DIAG_OUT_H
#define STONE_DIAG_OUT_H

namespace stone {
/// \brief Represents the severity level of a diagnostic.
enum class EmitLevel : unsigned {
  None = 0, ///< Default diagnostic level until chosen.
  Hide,     ///< Used for suppressing or deferring diagnostics (e.g., during
            ///< speculative parsing).
  Note,     ///< Informational note (often follows a warning or error).
  Warn,     ///< Warning that does not stop compilation.
  Error     ///< Error that prevents successful compilation.
};

enum class DiagReason : unsigned {
  Syntax,      ///< Grammatical errors or invalid token usage.
  Typing,      ///< Type errors (e.g., mismatches, inference failures).
  Logic,       ///< Violations of semantic rules (e.g., uninitialized vars).
  Style,       ///< Style issues, idiom violations, or redundancy.
  Performance, ///< Performance pitfalls or optimization notes.
  Portability, ///< Platform- or architecture-specific warnings.
  Deprecation, ///< Use of deprecated language features or APIs.
  Tooling,     ///< Emitted by tooling-related passes (e.g., fix-its, linters).
};

enum class EmitTime : unsigned {
  Now,          // Emit immediately
  AfterParse,   // Emit after parsing completes
  AfterLogic,   // Emit after logical analysis
  AfterCodeGen, // Emit after code generation
  Final,        // Emit at the very end of compilation
  Deferred      // Only emit if manually triggered
};

enum class EmitMedium : unsigned {
  Console,    // Emit to standard error (for users during CLI usage)
  File,       // Emit to an output file (e.g. diagnostics.json, .txt)
  IDE,        // Emit structured info for IDE tooling (e.g. LSP)
  Log,        // Emit for internal logging (e.g. compiler trace logs)
  Suppressed, // Emit silently but store in diagnostic cache
  API,        // Emit via API hooks (e.g. plugin or diagnostic client)
};
} // namespace stone

#endif
