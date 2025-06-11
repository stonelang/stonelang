set(stone_parse_sources
  Parser.cpp
  ParseDecl.cpp  
  ParseExpr.cpp  
  Parser.cpp     
  ParseStmt.cpp
  
  LINK_LIBS
  StoneAST

  DEPENDS
  StoneOptions

)
add_stone_library(StoneParse
  ${stone_parse_sources}
)