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

set(stone_support_sources
  About.cpp
  FileSystem.cpp
  FileSystemStatCache.cpp
  FileType.cpp
  Help.cpp
  InputFile.cpp
  LangOptions.cpp
  Options.cpp
  Platform.cpp
  SrcMgr.cpp
  Statistics.cpp
  Version.cpp
  
  DEPENDS
  StoneOptions

)
add_stone_library(StoneSupport
  ${stone_support_sources}
)

if (TARGET obj.StoneSupport)
  add_library(StoneOption_tablegen ALIAS obj.StoneSupport)
elseif (NOT LLVM_LINK_LLVM_DYLIB)
  add_library(StoneSupport_tablegen ALIAS StoneSupport)
else()
  # Build a version of the option library that does not link against
  # libLLVM-*.so, to be used by stone-tblgen. This is so stone-tblgen doesn't
  # link against libLLVMOption twice (once statically and once via
  # libLLVM-*.so).
  add_llvm_library(StoneSupport_tablegen
    BUILDTREE_ONLY STATIC DISABLE_LLVM_LINK_LLVM_DYLIB
    ${stone_support_sources})
endif()

 