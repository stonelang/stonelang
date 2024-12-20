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

add_stone_library(StoneFrontend
 FrontendInstance.cpp   
 FrontendInvocation.cpp 
 FrontendOptions.cpp

  DEPENDS
  StoneOptions

  LINK_LIBS
 
  ${system_libs}
  )
