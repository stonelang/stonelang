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

add_stone_library(StoneCodeGen
  
  CodeGenContext.cpp

  DEPENDS
  StoneOptions
   
  )
