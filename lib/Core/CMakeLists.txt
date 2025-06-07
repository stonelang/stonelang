set( LLVM_LINK_COMPONENTS
  ${LLVM_TARGETS_TO_BUILD}
	BitReader
	BinaryFormat
  BitstreamReader
  Support
  Core
  IPO
  Option
  )
set(stone_core_sources
	About.cpp
  InputFile.cpp
  Statistics.cpp
)
add_stone_library(StoneCore
	${stone_core_sources}
)



