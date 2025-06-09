#ifndef STONE_FRONTEND_FRONTENDINVOCATION_H
#define STONE_FRONTEND_FRONTENDINVOCATION_H

#include "stone/Frontend/FrontendOptions.h"
#include "stone/Option/CodeGenOptions.h"
#include "stone/Option/DiagnosticOptions.h"
#include "stone/Option/LangOptions.h"

namespace stone {

class FrontendInvocation final {
  FrontendOptions frontendOpts;
  LangOptions langOpts;
  CodeGenOptions codeGenOpts;
  DiagnosticOptions diagOpts;

public:
  FrontendInvocation();
};

} // namespace stone

#endif // STONE_FRONTEND_FRONTENDINPUTFILE_H
