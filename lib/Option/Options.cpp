 
#include "stone/Option/Options.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/Option/OptTable.h"
#include "llvm/Option/Option.h"
#include <cassert>

using namespace stone;
using namespace stone::opts;
using namespace llvm::opt;

#define OPTTABLE_VALUES_CODE
#include "stone/Option/Options.inc"
#undef OPTTABLE_VALUES_CODE

#define PREFIX(NAME, VALUE)                                                    \
  static constexpr llvm::StringLiteral NAME##_init[] = VALUE;                  \
  static constexpr llvm::ArrayRef<llvm::StringLiteral> NAME(                   \
      NAME##_init, std::size(NAME##_init) - 1);
#include "stone/Option/Options.inc"
#undef PREFIX

static constexpr const llvm::StringLiteral PrefixTable_init[] =
#define PREFIX_UNION(VALUES) VALUES
#include "stone/Option/Options.inc"
#undef PREFIX_UNION
    ;
static constexpr const llvm::ArrayRef<llvm::StringLiteral>
    PrefixTable(PrefixTable_init, std::size(PrefixTable_init) - 1);

static constexpr OptTable::Info InfoTable[] = {
#define OPTION(...) LLVM_CONSTRUCT_OPT_INFO(__VA_ARGS__),
#include "stone/Option/Options.inc"
#undef OPTION
};

namespace {
class StoneOptTable : public PrecomputedOptTable {
public:
  StoneOptTable() : PrecomputedOptTable(InfoTable, PrefixTable) {}
};
}

std::unique_ptr<OptTable> stone::CreateStoneOptTable() {
  return std::unique_ptr<OptTable>(new StoneOptTable());
}