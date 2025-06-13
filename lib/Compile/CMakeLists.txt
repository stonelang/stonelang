set(stone_compile_sources
  Compile.cpp
  
  LINK_LIBS
  StoneCodeGen
  StoneParse
  StoneFrontend
  clangFrontend

  DEPENDS
  StoneOptions
)
add_stone_library(StoneCompile
  ${stone_compile_sources}
)



