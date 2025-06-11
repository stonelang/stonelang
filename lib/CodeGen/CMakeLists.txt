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
set(stone_codegen_sources
		CodeGeneration.cpp 
		CodeGenModule.cpp  
		CodeGenPasses.cpp
		 
		LINK_LIBS
		StoneAST
		clangCodeGen;
)
add_stone_library(StoneCodeGen
	${stone_codegen_sources}
)


