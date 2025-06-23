#===----------------------------------------------------------------------===#
# Link against LLVM components
#===----------------------------------------------------------------------===#
set(LLVM_LINK_COMPONENTS
  BinaryFormat
  Core
  Support
)

#===----------------------------------------------------------------------===#
# Stone AST Sources
#===----------------------------------------------------------------------===#
set(stone_ast_sources
  Decl.cpp
  DeclFlight.cpp
  Identifier.cpp
  Module.cpp
  ModuleContext.cpp
  File.cpp
  Node.cpp
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
)

#===----------------------------------------------------------------------===#
# Add StoneAST library
#===----------------------------------------------------------------------===#
add_stone_library(StoneAST
  ${stone_ast_sources}

  LINK_LIBS
    StoneCore
    clangAST
    clangBasic
)

target_include_directories(StoneAST
  PRIVATE
    ${LLVM_BINARY_DIR}/tools/clang/include
    ${LLVM_SOURCE_DIR}/tools/clang/include
)
