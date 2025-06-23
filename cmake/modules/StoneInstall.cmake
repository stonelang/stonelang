#===----------------------------------------------------------------------===//
# Stone Installation Configuration
#===----------------------------------------------------------------------===//

# Install public headers (source)
install(DIRECTORY include/stone include/stone-c
  DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
  COMPONENT stone-headers
  FILES_MATCHING
    PATTERN "*.def"
    PATTERN "*.h"
    PATTERN "config.h" EXCLUDE)

# Install public headers (generated)
install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/include/stone
  DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
  COMPONENT stone-headers
  FILES_MATCHING
    PATTERN "*.h"
    PATTERN "*.inc"
    PATTERN "CMakeFiles" EXCLUDE)

# Install dependency: stone-tblgen headers must be generated first
add_custom_target(stone-headers DEPENDS stone-tablegen-targets)
set_target_properties(stone-headers PROPERTIES FOLDER "Stone/Resources")

if(NOT LLVM_ENABLE_IDE)
  add_llvm_install_targets(install-stone-headers
    DEPENDS stone-headers
    COMPONENT stone-headers)
endif()

# Bash autocomplete script
set_target_properties(bash-autocomplete PROPERTIES FOLDER "Stone/Misc")

install(FILES utils/bash-autocomplete.sh
  DESTINATION "${CMAKE_INSTALL_DATADIR}/stone"
  COMPONENT bash-autocomplete)

# Uncomment to install autocomplete as part of install target
# if(NOT LLVM_ENABLE_IDE)
#   add_llvm_install_targets(install-bash-autocomplete
#     DEPENDS bash-autocomplete
#     COMPONENT bash-autocomplete)
# endif()

# Master install target for all stone libraries
add_custom_target(stone-libraries)
set_target_properties(stone-libraries PROPERTIES FOLDER "Stone/Install")

if(NOT LLVM_ENABLE_IDE)
  add_llvm_install_targets(install-stone-libraries
    DEPENDS stone-libraries
    COMPONENT stone-libraries)
endif()

# Add all registered Stone libs to install dependencies
get_property(STONE_LIBS GLOBAL PROPERTY STONE_LIBS)
if(STONE_LIBS)
  list(REMOVE_DUPLICATES STONE_LIBS)
  foreach(lib ${STONE_LIBS})
    add_dependencies(stone-libraries ${lib})
    if(NOT LLVM_ENABLE_IDE)
      add_dependencies(install-stone-libraries install-${lib})
      add_dependencies(install-stone-libraries-stripped install-${lib}-stripped)
    endif()
  endforeach()
endif()
