#ifndef STONE_AST_DIAGNOSTICPARSE_H
#define STONE_AST_DIAGNOSTICSCOMMON_H

#include "stone/AST/DiagnosticBasicKind.h"
 
namespace stone {
namespace diags {
#define DIAG(ENUM, CLASS, DEFAULT_SEVERITY, DESC, GROUP, SFINAE, NOWERROR,SHOWINSYSHEADER, SHOWINSYSMACRO, DEFERRABLE, CATEGORY)                               \
  extern Diag::ID ENUM;
#include "stone/AST/DiagnosticParseKind.inc"

} // end namespace diag
} // end namespace stone

#endif 
