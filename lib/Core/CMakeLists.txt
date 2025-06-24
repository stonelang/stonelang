#===----------------------------------------------------------------------===//
# StoneCore - Core support library for Stone
#===----------------------------------------------------------------------===//

# LLVM dependencies required by this component
set(LLVM_LINK_COMPONENTS
  ${LLVM_TARGETS_TO_BUILD}
  BitReader
  BinaryFormat
  BitstreamReader
  Option
  Core
  IPO
)

# Remove stone-tablegen-targets from LLVM_COMMON_DEPENDS
# so that StoneOptions can be used inside stone-tblgen
list(REMOVE_ITEM LLVM_COMMON_DEPENDS stone-tablegen-targets)

# StoneCore source files
set(stone_support_sources
  About.cpp
  Algorithm.cpp
  CharInfo.cpp
  DiagEngine.cpp
  EditorPlaceholder.cpp
  FileSystem.cpp
  FileSystemStatCache.cpp
  FileType.cpp
  FixItID.cpp
  Help.cpp
  LangOptions.cpp
  Options.cpp
  Platform.cpp
  SrcMgr.cpp
  SrcUnit.cpp
  Statistics.cpp
  Unicode.cpp
  Version.cpp
)

# Core library build
add_stone_library(StoneCore
  ${stone_support_sources}
  DEPENDS
    StoneOptions
)

# Tablegen aliasing logic
if(TARGET obj.StoneCore)
  add_library(StoneOption_tablegen ALIAS obj.StoneCore)
elseif(NOT LLVM_LINK_LLVM_DYLIB)
  add_library(StoneCore_tablegen ALIAS StoneCore)
else()
  # If we're linking libLLVM dynamically, build a standalone static lib
  add_llvm_library(StoneCore_tablegen
    BUILDTREE_ONLY
    STATIC
    DISABLE_LLVM_LINK_LLVM_DYLIB
    ${stone_support_sources})
endif()
