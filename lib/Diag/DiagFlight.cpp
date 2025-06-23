#include "DiagFlight.h"

namespace stone {

#define DIAG(ID, CODE, TEXT, SIG)                                              \
  static DiagFlight ID##Flight = {DiagID::ID, CODE, TEXT};

#include "stone/Core/Diagnostics.def"

static DiagFlight *DiagFlightTable[] = {
#define DIAG(ID, CODE, TEXT, SIG) &ID##Flight,
#include "stone/Core/Diagnostics.def"
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

#include "FixItFlight.h"

namespace stone {

#define FIXIT(ID, TEXT, SIG)                                                   \
  static constexpr auto ID##Args = MakeFixItArgumentKindArray SIG();           \
  static const FixItFlight ID##FixFlight = {                                   \
      FixItID::ID, TEXT, ID##Args,                                             \
      sizeof(ID##Args) / sizeof(DiagnosticArgumentKind)};

#include "stone/Core/Diagnostics.def"

static const FixItFlight *fixItTable[] = {
#define FIXIT(ID, TEXT, SIG) &ID##FixFlight,
#include "stone/Core/Diagnostics.def"
};

const FixItFlight *GetFixItFlight(FixItID id) {
  return fixItTable[static_cast<uint32_t>(id)];
}

} // namespace stone
