#ifndef STONE_DRIVER_RUN_H
#define STONE_DRIVER_RUN_H

#include "stone/Core/Status.h"

#include "llvm/ADT/ArrayRef.h"

namespace stone {
class Driver;

/// \return 0 if run was successful or 1 if not successful
int Run(llvm::ArrayRef<const char *> args, const char *arg0, void *mainAddr);

/// \return true if run was successful
Status Run(Driver &driver);

} // namespace stone

#endif
