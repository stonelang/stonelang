#ifndef STONE_AST_DIAGNOSTICSCOMMON_H
#define STONE_AST_DIAGNOSTICSCOMMON_H

#include "stone/AST/DiagnosticID.h"
 

namespace stone {
namespace diags {

struct Diag;
#define DIAG(ENUM, CLASS, DEFAULT_SEVERITY, DESC, GROUP, SFINAE, NOWERROR,SHOWINSYSHEADER, SHOWINSYSMACRO, DEFERRABLE, CATEGORY)                               \
  extern Diag::ID ENUM;
#include "stone/AST/DiagnosticBasicKind.inc"

} // end namespace diag
} // end namespace stone

#endif 
