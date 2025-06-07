set(stone_frontend_sources
  FrontendInstance.cpp
  FrontendInvocation.cpp
  FrontendOptions.cpp
  
  LINK_LIBS

  StoneOption
  clangFrontend

  DEPENDS
  StoneOptions
)
add_stone_library(StoneFrontend
  ${stone_frontend_sources}
)



