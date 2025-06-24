#ifndef STONE_DIAG_FIXIT_FLIGHT_H
#define STONE_DIAG_FIXIT_FLIGHT_H

#include "stone/Diag/DiagArtifact.h"
#include "stone/Diag/FixItID.h"

namespace stone {

class FixItFlight final {
  FixItID fixItID;
  llvm::StringRef message;
  const DiagArtifactKind *args;
  size_t numArgs;

public:
  FixItFlight();
};

template <typename... ArgTypes> constexpr auto MakeFixItArtifactKinds() {
  static constexpr DiagArtifactKind kinds[] = {
      DiagArtifactKindOf<ArgTypes>::kind...};
  return kinds;
}

const FixItFlight *GetFixItFlight(FixItID ID);

} // namespace stone
