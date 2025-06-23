#ifndef STONE_DIAG_DIAGID_H
#define STONE_DIAG_DIAGID_H

#include <cstdint>

namespace stone {
namespace diag {
enum class DiagID : uint32_t {
#define DIAG(ID, CODE, MESSAGE, SIG) ID,
#include "stone/Core/DiagID.def"
  Count
};
} // namespace diag
} // namespace stone
#endif 
