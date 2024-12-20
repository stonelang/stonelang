#ifndef STONE_FRONTEND_FRONTEND_H
#define STONE_FRONTEND_FRONTEND_H

#include "stone/Frontend/FrontendOptions.h"

#include <memory>
namespace stone {

class FrontendInvocation {
public:
  FrontendInvocation();
};

class FrontendInstance {
  std::unique_ptr<FrontendInvocation> invocation;

public:
  FrontendInstance();

public:
};

} // namespace stone

#endif
