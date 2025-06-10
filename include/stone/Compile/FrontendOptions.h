#ifndef STONE_FRONTEND_FRONTENDOPTIONS_H
#define STONE_FRONTEND_FRONTENDOPTIONS_H

namespace stone {

enum class FrontendMode : unsigned {
  None = 0,
#define FRONTEND_MODE(A) A,
#include "stone/Lang/Mode.def"
};
class FrontendOptions final {
  FrontendMode frontendMode;

public:
  FrontendOptions();

public:
  FrontendMode GetMode() { return frontendMode; }
};

} // namespace stone

#endif // STONE_FRONTEND_FRONTENDINPUTFILE_H
