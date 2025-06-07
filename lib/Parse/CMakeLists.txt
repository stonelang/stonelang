set(stone_parse_sources
  
  Parser.cpp
  
  LINK_LIBS
  StoneAST
  StoneCore

)
add_stone_library(StoneParse
  ${stone_parse_sources}
)



