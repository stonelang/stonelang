#pragma once

#include "DiagID.h"
#include "DiagnosticArgumentKind.h"
#include "FixItFlight.h"
#include "llvm/ADT/SmallVector.h"

namespace stone {

struct DiagFlight {
  DiagID id;
  const char *code; // e.g. "E001"
  const char *message;
  const DiagnosticArgumentKind *args;
  size_t numArgs;
  llvm::SmallVector<const FixItFlight *, 1> fixIts;

  DiagFlight &AddFixIt(FixItID id);
};

template <typename... ArgTypes> constexpr auto MakeArgumentKindArray() {
  static constexpr DiagnosticArgumentKind kinds[] = {
      DiagnosticArgumentKindOf<ArgTypes>::kind...};
  return kinds;
}

const DiagFlight *GetDiagFlight(DiagID id);

} // namespace stone
