set(LLVM_LINK_COMPONENTS
  BinaryFormat
  Core
  Support
 )

set(stone_ast_sources
	Decl.cpp
	DeclFlight.cpp
	Identifier.cpp
	Module.cpp
	ModuleContext.cpp
	File.cpp
	Node.cpp
	DeclFlight.cpp
	Diagnostics.cpp
	DiagnosticID.cpp
	EvaluatorEngine.cpp
	Expr.cpp
	Scaffolder.cpp
	Scope.cpp
	Stmt.cpp
	Type.cpp
	Logic.cpp
	TypeFlight.cpp
	Visibility.cpp

	LINK_LIBS
	StoneCore

)
add_stone_library(StoneAST
	${stone_ast_sources}
)


