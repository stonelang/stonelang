set(LLVM_LINK_COMPONENTS
  BinaryFormat
  Core
  Support
 )

set(stone_ast_sources
	ASTSession.cpp
	ASTUnit.cpp
	ASTFile.cpp
	Decl.cpp
	Diagnostics.cpp
	DiagnosticID.cpp
	Evaluator.cpp
	Expr.cpp
	Space.cpp
	Scaffolder.cpp
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


