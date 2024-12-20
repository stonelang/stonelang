set(LLVM_LINK_COMPONENTS
  BinaryFormat
  Core
  FrontendOpenMP
  Support
  TargetParser
  )

add_stone_library(StoneAST
  ASTContext.cpp  
  ClangImporter.cpp 
  DiagnosticEngine.cpp
  DiagnosticID.cpp
  Identifier.cpp
  ModuleDecl.cpp
  LINK_LIBS

  DEPENDS
  omp_gen
  StoneOptions
)


target_link_libraries(StoneAST
  PRIVATE
  ${LLVM_ATOMIC_LIB}
)

