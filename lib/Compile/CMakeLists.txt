set(stone_compile_sources
  Compile.cpp
  Compiling.cpp
  Frontend.cpp
  FrontendOptions.cpp

  
  LINK_LIBS
  StoneCodeGen
  StoneParse
  clangFrontend

  DEPENDS
  StoneOptions
)
add_stone_library(StoneCompile
  ${stone_compile_sources}
)



