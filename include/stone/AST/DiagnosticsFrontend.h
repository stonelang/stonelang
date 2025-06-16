#ifndef STONE_DIAG_DIAGNOSTICS_FRONTEND_H
#define STONE_DIAG_DIAGNOSTICS_FRONTEND_H

#include "stone/AST/DiagnosticsCore.h"

namespace stone {
namespace diag {
// Declare common diagnostics objects with their appropriate types.
#define DIAG(KIND, ID, Options, Text, Signature)                               \
  extern detail::DiagWithArguments<void Signature>::type ID;
#include "stone/Support/DiagnosticsCompile.def"
} // namespace diag
} // namespace stone

#endif
