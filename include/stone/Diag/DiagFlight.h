#ifndef STONE_DIAG_FLIGHT_KIND_H
#define STONE_DIAG_FLIGHT_KIND_H

#include "DiagID.h"
#include "DiagnosticArgumentKind.h"
#include "FixItFlight.h"
#include "llvm/ADT/SmallVector.h"

namespace stone {

class DiagFlight {
  DiagID id;
  llvm::StringRef code; // e.g. "E001"
  llvm::StringRef message;
  const DiagArtifactKind *args;
  size_t numArgs;
  llvm::SmallVector<const FixItFlight *, 1> fixIts;

public:
  DiagFlight();
  DiagFlight &AddFixIt(FixItID fixItID);
};

template <typename... ArgTypes> constexpr auto MakeDiagArtifactKinds() {
  static constexpr DiagArtifactKind kinds[] = {
      DiagArtifactKindOf<ArgTypes>::kind...};
  return kinds;
}

const DiagFlight *GetDiagFlight(DiagID id);

} // namespace stone
#endif
