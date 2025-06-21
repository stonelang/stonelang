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
		LLVMCodeGenModule.cpp 
		LLVMPasses.cpp
		 
		LINK_LIBS
		StoneAST
		clangCodeGen;
)
add_stone_library(StoneLLVM
	${stone_codegen_sources}
)


