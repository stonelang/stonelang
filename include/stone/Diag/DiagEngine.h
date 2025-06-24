#ifndef STONE_DIAG_ENGINE_H
#define STONE_DIAG_ENGINE_H

#include "stone/Diag/DiagArtifact.h"
#include "stone/Diag/DiagID.h"
#include "stone/Diag/FixItFlight.h"

#include "llvm/ADT/SmallVector.h"

namespace stone {

class DiagEngine final {
  DiagOptions &diagOpts;

public:
  DiagEngine(DiagOptions &diagOpts);

public:
  void Emit();
  void Emit(EmitTime time);

public:
};

} // namespace stone
#endif
