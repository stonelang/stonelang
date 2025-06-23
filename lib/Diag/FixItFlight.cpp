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
