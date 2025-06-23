#pragma once

#include "DiagID.h"
#include "DiagnosticArgumentKind.h"

namespace stone {

struct FixItFlight {
  FixItID id;
  const char *message;
  const DiagnosticArgumentKind *args;
  size_t numArgs;
};

template <typename... ArgTypes>
constexpr auto MakeFixItArgumentKindArray() {
  static constexpr DiagnosticArgumentKind kinds[] = {
    DiagnosticArgumentKindOf<ArgTypes>::kind...
  };
  return kinds;
}

const FixItFlight *GetFixItFlight(FixItID id);

} // namespace stone
