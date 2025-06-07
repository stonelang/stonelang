set(stone_drive_sources
  Driver.cpp
  LINK_LIBS
  StoneCore
  StoneOption
)
add_stone_library(StoneDriver
  ${stone_drive_sources}

  DEPENDS
  StoneOptions
)

