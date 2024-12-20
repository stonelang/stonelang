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

add_stone_library(StoneDriver
   
  Driver.cpp
  

  DEPENDS
  StoneOptions
  

  LINK_LIBS
 
  ${system_libs}
  )
