set(LLVM_LINK_COMPONENTS
  BinaryFormat
  MC
  Object
  Option
  ProfileData
  Support
  TargetParser
  WindowsDriver
  )

add_stone_library(StoneCompile
   
  Compile.cpp
  Parser.cpp
  Sem.cpp
  
  DEPENDS
  StoneOptions
  StoneFrontend
  
  
  LINK_LIBS
 
  ${system_libs}
  )
