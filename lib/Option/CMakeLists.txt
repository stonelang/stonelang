set( LLVM_LINK_COMPONENTS
  ${LLVM_TARGETS_TO_BUILD}
	BitReader
	BinaryFormat
  BitstreamReader
  Option
  Core
  IPO
  Option
 )

# Drop stone-tablegen-targets from LLVM_COMMON_DEPENDS.
# so that we could use StoneOption within stone-tblgen and other stone
# component.
list(REMOVE_ITEM LLVM_COMMON_DEPENDS stone-tablegen-targets)


set(stone_option_sources

  Options.cpp

	LINK_LIBS 
	StoneCore

  DEPENDS
  StoneOptions

)
add_stone_library(StoneOption ${stone_option_sources})

if (TARGET obj.StoneOption)
  add_library(StoneOption_tablegen ALIAS obj.StoneOption)
elseif (NOT LLVM_LINK_LLVM_DYLIB)
  add_library(StoneOption_tablegen ALIAS StoneOption)
else()
  # Build a version of the option library that does not link against
  # libLLVM-*.so, to be used by stone-tblgen. This is so stone-tblgen doesn't
  # link against libLLVMOption twice (once statically and once via
  # libLLVM-*.so).
  add_llvm_library(StoneOption_tablegen
    BUILDTREE_ONLY STATIC DISABLE_LLVM_LINK_LLVM_DYLIB
    ${stone_option_sources})
endif()

 