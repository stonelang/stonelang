set(LLVM_LINK_COMPONENTS
  BinaryFormat
  Core
  Support
 )

set(stone_ast_sources
	Artifact.cpp
	AST.cpp
	Decl.cpp
	Module.cpp
	ModuleContext.cpp
	ModuleFile.cpp
	DeclState.cpp
	Diagnostics.cpp
	DiagnosticID.cpp
	Evaluator.cpp
	Expr.cpp
	Scaffolder.cpp
	Stmt.cpp
	Type.cpp
	TypeChecker.cpp
	TypeState.cpp
	Visibility.cpp

	LINK_LIBS
	StoneSupport

)
add_stone_library(StoneAST
	${stone_ast_sources}
)


