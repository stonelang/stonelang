set(LLVM_LINK_COMPONENTS
  BinaryFormat
  Core
  FrontendOpenMP
  Support
  TargetParser
  )

add_stone_library(stoneAST
  AST.cpp   
  DiagnosticEngine.cpp
  DiagnosticID.cpp
  LINK_LIBS

  DEPENDS
  omp_gen
  StoneOptions
)


target_link_libraries(stoneAST
  PRIVATE
  ${LLVM_ATOMIC_LIB}
)

