#ifndef STONE_DIAG_DIAGID_H
#define STONE_DIAG_DIAGID_H

#include <cstdint>

namespace stone {
namespace diag {
enum class FixItID : uint32_t {
#define FIXIT(ID, MESSAGE, SIG) ID,
#include "stone/Core/DiagID.def"
  Count
};
} // namespace diag
} // namespace stone
#endif
