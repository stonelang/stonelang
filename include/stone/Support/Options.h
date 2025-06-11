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
  ExcludeFrontendOption = (1 << 5),
  DriverOption = (1 << 6),
  ExcludeDriverOption = (1 << 7),
  DebugOption = (1 << 8),
  ArgumentIsPath = (1 << 9),
  ModuleInterfaceOption = (1 << 10),
  SupplementaryOutput = (1 << 11),
  StoneAPIExtractOption = (1 << 12),
  StoneSymbolGraphExtractOption = (1 << 13),
  StoneAPIDigesterOption = (1 << 14),
  ExcludeBatchOption = (1 << 15),
  DoesNotAffectIncrementalBuild = (1 << 16),
  CacheInvariant = (1 << 17),
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
