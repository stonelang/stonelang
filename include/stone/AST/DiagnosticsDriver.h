#ifndef STONE_DIAG_DIAGNOSTIC_DRIVER_H
#define STONE_DIAG_DIAGNOSTIC_DRIVER_H

#include "stone/AST/DiagnosticsCore.h"

namespace stone {
namespace diag {
// Declare common diagnostics objects with their appropriate types.
#define DIAG(KIND, ID, Options, Text, Signature)                               \
  extern detail::DiagWithArguments<void Signature>::type ID;
#include "stone/Support/DiagnosticsDriver.def"
} // namespace diag
} // namespace stone

#endif
