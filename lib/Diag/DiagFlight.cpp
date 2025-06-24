#include "stone/Diag/DiagFlight.h"

namespace stone {

#define DIAG(ID, CODE, TEXT, SIG)                                              \
  static DiagFlight ID##Flight = {DiagID::ID, CODE, TEXT};
#include "stone/Diag/DiagID.def"

static DiagFlight *DiagFlightTable[] = {
#define DIAG(ID, CODE, TEXT, SIG) &ID##Flight,
#include "stone/Diag/DiagID.def"
};

const DiagFlight *GetDiagFlight(DiagID id) {
  return DiagFlightTable[static_cast<uint32_t>(id)];
}

DiagFlight &DiagFlight::AddFixIt(FixItID id) {
  if (auto *fix = GetFixItFlight(id)) {
    fixIts.push_back(fix);
  }
  return *this;
}

} // namespace stone
