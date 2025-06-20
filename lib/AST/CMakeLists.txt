set(LLVM_LINK_COMPONENTS
  BinaryFormat
  Core
  Support
 )

set(stone_ast_sources
	Artifact.cpp
	Decl.cpp
	Module.cpp
	ModuleContext.cpp
	ModuleFile.cpp
	Node.cpp
	DeclState.cpp
	Diagnostics.cpp
	DiagnosticID.cpp
	EvaluatorEngine.cpp
	Expr.cpp
	Scaffolder.cpp
	Scope.cpp
	Stmt.cpp
	Type.cpp
	Logic.cpp
	TypeState.cpp
	Visibility.cpp

	LINK_LIBS
	StoneSupport

)
add_stone_library(StoneAST
	${stone_ast_sources}
)


