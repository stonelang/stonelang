#include "stone/AST/DiagnosticID.h"

using namespace stone;

// Get an official count of all of the diagnostics in the system
enum LocalDiagID : uint32_t {
#define DIAG(ENUM, CLASS, DEFAULT_SEVERITY, DESC, GROUP, SFINAE, NOWERROR,     \
             SHOWINSYSHEADER, SHOWINSYSMACRO, DEFERRABLE, CATEGORY)            \
  ENUM,
#include "stone/AST/AllDiagnosticKind.inc"
  TotalDiags
};