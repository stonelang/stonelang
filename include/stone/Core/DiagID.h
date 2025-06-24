#ifndef STONE_DIAG_DIAGID_H
#define STONE_DIAG_DIAGID_H

#include <cstdint>

namespace stone {
namespace diag {

enum class DiagID : uint32_t {
  None = 0,
#define DIAG(ID, CODE, MESSAGE, SIG) ID,
#include "stone/Core/DiagEngine.def"
  Count
};

} // namespace diag
} // namespace stone

#endif // STONE_DIAG_DIAGID_H

// Emit by ID + args (standard path)
// template <typename... Args>
// void Emit(DiagID id, Args &&...args);

// // Emit using preconstructed flight (custom or canonical)
// template <typename... Args>
// void EmitDiagFlight(const DiagFlight &flight, Args &&...args);

// // Emit FixIt directly by ID
// template <typename... Args>
// void EmitFixIt(FixItID id, Args &&...args);

// // Emit prebuilt fix-it
// template <typename... Args>
// void EmitFixItFlight(const FixItFlight &flight, Args &&...args);

// // Internally resolves to:
// EmitDiagFlight(GetDiagFlight(id), std::forward<Args>(args)...);
// EmitFixItFlight(GetFixItFlight(id), std::forward<Args>(args)...);
