set(stone_compile_sources
  Compile.cpp
  Frontend.cpp
  FrontendArgStrings.cpp
  FrontendModule.cpp
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



