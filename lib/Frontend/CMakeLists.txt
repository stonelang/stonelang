set(stone_frontend_sources
  Frontend.cpp
  FrontendArgStrings.cpp
  FrontendModule.cpp
  FrontendOptions.cpp
  
  LINK_LIBS

  StoneAST
  clangFrontend

  DEPENDS
  StoneOptions
)
add_stone_library(StoneFrontend
  ${stone_frontend_sources}
)



