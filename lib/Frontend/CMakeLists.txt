set(stone_frontend_sources
  Frontend.cpp
  FrontendArgList.cpp
  FrontendSpace.cpp
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



