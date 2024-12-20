set(LLVM_COMMON_DEPENDS_OLD ${LLVM_COMMON_DEPENDS})

# Drop stone-tablegen-targets from LLVM_COMMON_DEPENDS.
# so that we could use stoneSupport within stone-tblgen and other stone
# component.
list(REMOVE_ITEM LLVM_COMMON_DEPENDS stone-tablegen-targets)

set(LLVM_LINK_COMPONENTS
  Support
  )

set(stoneSupport_sources
  RISCVVIntrinsicUtils.cpp
  )

add_stone_library(stoneSupport ${stoneSupport_sources})

if (TARGET obj.stoneSupport)
  add_library(stoneSupport_tablegen ALIAS obj.stoneSupport)
elseif (NOT LLVM_LINK_LLVM_DYLIB)
  add_library(stoneSupport_tablegen ALIAS stoneSupport)
else()
  # Build a version of the support library that does not link against
  # libLLVM-*.so, to be used by stone-tblgen. This is so stone-tblgen doesn't
  # link against libLLVMSupport twice (once statically and once via
  # libLLVM-*.so).
  add_llvm_library(stoneSupport_tablegen
    BUILDTREE_ONLY STATIC DISABLE_LLVM_LINK_LLVM_DYLIB
    ${stoneSupport_sources})
endif()

set(LLVM_COMMON_DEPENDS ${LLVM_COMMON_DEPENDS_OLD})
