set(stone_parse_sources
  Confusable.cpp
  Lexer.cpp 
  ParseDecl.cpp  
  ParseExpr.cpp  
  Parser.cpp   
  ParseStmt.cpp
  ParseType.cpp
      
  LINK_LIBS
  StoneAST

  DEPENDS
  StoneOptions

)
add_stone_library(StoneParse
  ${stone_parse_sources}
)