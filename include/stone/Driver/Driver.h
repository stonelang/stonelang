#ifndef STONE_DRIVER_DRIVER_H
#define STONE_DRIVER_DRIVER_H

#include "stone/Core/LangOptions.h"
#include "stone/Core/Statistics.h"
#include "stone/Core/Status.h"

#include "llvm/ADT/DenseMap.h"
#include "llvm/ADT/SetVector.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/Support/Allocator.h"

#include <functional>
#include <memory>
#include <string>

namespace llvm {
namespace opt {
class Arg;
class ArgList;
class OptTable;
class InputArgList;
class DerivedArgList;
} // namespace opt
} // namespace llvm

namespace stone {

class Driver final {
  /// The base set of options
  LangOptions langOpts;

public:
  Driver(llvm::StringRef executablePath, llvm::StringRef executableNam);

public:
  Status ParseArgStrings(llvm::ArrayRef<const char *> args);
};

} // namespace stone
#endif