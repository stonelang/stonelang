set(LLVM_LINK_COMPONENTS
  BinaryFormat
  Core
  Support
 )

set(stone_ast_sources
	ASTBuilder.cpp
	ASTSession.cpp
	ASTUnit.cpp
	Decl.cpp
	Expr.cpp
	ModuleDecl.cpp
	ModuleFile.cpp
	Stmt.cpp
	Type.cpp
	TypeChecker.cpp
	Visibility.cpp

	LINK_LIBS
	StoneSupport

)
add_stone_library(StoneAST
	${stone_ast_sources}
)


