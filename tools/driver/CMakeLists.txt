set( LLVM_LINK_COMPONENTS
  ${LLVM_TARGETS_TO_BUILD}
  Analysis
  CodeGen
  Core
  IPO
  AggressiveInstCombine
  InstCombine
  Instrumentation
  MC
  MCParser
  ObjCARCOpts
  Option
  ScalarOpts
  Support
  TargetParser
  TransformUtils
  Vectorize
  )

add_stone_tool(stone
  main.cpp

   DEPENDS
  StoneOptions

  
  
)


