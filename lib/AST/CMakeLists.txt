set(LLVM_LINK_COMPONENTS
  BinaryFormat
  Core
  Support
 )

set(stone_ast_sources
	ASTSession.cpp
	ASTUnit.cpp
	Decl.cpp
	DeclBuilder.cpp
	Expr.cpp
	ExprBuilder.cpp
	ModuleDecl.cpp
	Scaffolder.cpp
	SourceFile.cpp
	Stmt.cpp
	StmtBuilder.cpp
	Type.cpp
	TypeBuilder.cpp
	TypeChecker.cpp
	Visibility.cpp

	LINK_LIBS
	StoneSupport

)
add_stone_library(StoneAST
	${stone_ast_sources}
)


