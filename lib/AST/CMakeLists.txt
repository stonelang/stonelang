set(LLVM_LINK_COMPONENTS
  BinaryFormat
  Core
  Support
 )

set(stone_ast_sources
	AST.cpp
	ASTContext.cpp
	Decl.cpp
	DeclContext.cpp
	Type.cpp

	LINK_LIBS
	StoneOption
)
add_stone_library(StoneAST
	${stone_ast_sources}
)


