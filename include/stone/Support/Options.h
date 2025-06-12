#ifndef STONE_SUPPORT_OPTIONS_H
#define STONE_SUPPORT_OPTIONS_H

#include "llvm/Option/Arg.h"
#include "llvm/Option/ArgList.h"
#include "llvm/Option/Option.h"
#include "llvm/TargetParser/Triple.h"

#include <memory>

namespace llvm {
namespace opt {
class OptTable;
}
} // namespace llvm

namespace stone {
namespace opts {
enum OptFlag {
  FrontendOption = (1 << 4),
  DriverOption = (1 << 5),
  NotForDriverOption = (1 << 6),
  DebugOption = (1 << 7),
  ArgumentIsPath = (1 << 8),
  ModuleInterfaceOption = (1 << 9),
  SupplementaryOutput = (1 << 10),
  StoneAPIExtractOption = (1 << 11),
  StoneSymbolGraphExtractOption = (1 << 12),
  StoneAPIDigesterOption = (1 << 13),
  ExcludeBatchOption = (1 << 14),
  DoesNotAffectIncrementalBuild = (1 << 15),
  CacheInvariant = (1 << 16),
};

enum OptID {
  OPT_INVALID = 0, // This is not an option ID.
#define OPTION(...) LLVM_MAKE_OPT_ID(__VA_ARGS__),
#include "stone/Support/Options.inc"
  OPT_LAST
#undef OPTION
};

llvm::StringRef GetEqualValueByOptID(const opts::OptID optID,
                                     const llvm::opt::InputArgList &args);
unsigned GetArgID(const llvm::opt::Arg *arg);
llvm::StringRef GetArgName(const llvm::opt::Arg *arg);

} // namespace opts

std::unique_ptr<llvm::opt::OptTable> CreateOptTable();

} // namespace stone

#endif
