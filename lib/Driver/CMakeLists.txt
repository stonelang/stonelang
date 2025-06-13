set(stone_drive_sources

  Driver.cpp
  Run.cpp

  LINK_LIBS
  StoneAST
)
add_stone_library(StoneDriver
  ${stone_drive_sources}

  DEPENDS
  StoneOptions
)

