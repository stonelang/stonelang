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

add_stone_library(StoneBasic
   
  SourceLocation.cpp
  SourceManager.cpp
  
  DEPENDS
  StoneOptions
  
  
  LINK_LIBS
 
  ${system_libs}
)
