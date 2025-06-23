#include "stone/Core/Options.h"

#include "llvm/ADT/STLExtras.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/Option/OptTable.h"
#include "llvm/Option/Option.h"

#include "llvm/Support/FileSystem.h"
#include "llvm/Support/Path.h"
#include "llvm/TargetParser/Host.h"
#include "llvm/TargetParser/Triple.h"

using namespace stone;
using namespace stone::opts;

using namespace llvm::opt;

#define OPTTABLE_VALUES_CODE
#include "stone/Core/Options.inc"
#undef OPTTABLE_VALUES_CODE

#define PREFIX(NAME, VALUE)                                                    \
  static constexpr llvm::StringLiteral NAME##_init[] = VALUE;                  \
  static constexpr llvm::ArrayRef<llvm::StringLiteral> NAME(                   \
      NAME##_init, std::size(NAME##_init) - 1);
#include "stone/Core/Options.inc"
#undef PREFIX

static constexpr const llvm::StringLiteral PrefixTable_init[] =
#define PREFIX_UNION(VALUES) VALUES
#include "stone/Core/Options.inc"
#undef PREFIX_UNION
    ;
static constexpr const llvm::ArrayRef<llvm::StringLiteral>
    PrefixTable(PrefixTable_init, std::size(PrefixTable_init) - 1);

static constexpr llvm::opt::GenericOptTable::Info InfoTable[] = {
#define OPTION(...) LLVM_CONSTRUCT_OPT_INFO(__VA_ARGS__),
#include "stone/Core/Options.inc"
#undef OPTION
};

namespace {
class StoneOptTable : public llvm::opt::GenericOptTable {
public:
  StoneOptTable() : GenericOptTable(InfoTable) {}
};
} // end anonymous namespace

std::unique_ptr<llvm::opt::OptTable> stone::CreateOptTable() {
  return std::unique_ptr<llvm::opt::GenericOptTable>(new StoneOptTable());
}

llvm::StringRef
stone::opts::GetEqualValueByOptID(const opts::OptID optID,
                                  const llvm::opt::InputArgList &args) {}

unsigned stone::opts::GetArgID(const llvm::opt::Arg *arg) {}

llvm::StringRef stone::opts::GetArgName(const llvm::opt::Arg *arg) {}
