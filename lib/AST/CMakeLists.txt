set(LLVM_LINK_COMPONENTS
  BinaryFormat
  Core
  Support
 )

set(stone_ast_sources
	Decl.cpp

	LINK_LIBS
	StoneSupport
)
add_stone_library(StoneAST
	${stone_ast_sources}
)


