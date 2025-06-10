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
	Expr.cpp
	ModuleDecl.cpp
	ModuleFile.cpp
	Stmt.cpp
	Type.cpp
	TypeChecking.cpp
	Visibility.cpp


	LINK_LIBS
	StoneLang
)
add_stone_library(StoneAST
	${stone_ast_sources}
)


