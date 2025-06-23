include(GNUInstallDirs)
include(LLVMDistributionSupport)
include(ExtendPath)
include(FindPrefixFromConfig)

#===----------------------------------------------------------------------===//
# TableGen Support
#===----------------------------------------------------------------------===//

function(stone_tablegen)
  cmake_parse_arguments(CTG "" "SOURCE;TARGET" "" ${ARGN})
  if(NOT CTG_SOURCE)
    message(FATAL_ERROR "SOURCE source-file required by stone_tablegen")
  endif()

  set(STONE_TABLEGEN_ARGUMENTS "")
  set(LLVM_TARGET_DEFINITIONS ${CTG_SOURCE})
  tablegen(STONE ${CTG_UNPARSED_ARGUMENTS} ${STONE_TABLEGEN_ARGUMENTS})

  if(CTG_TARGET)
    add_public_tablegen_target(${CTG_TARGET})
    set_property(GLOBAL APPEND PROPERTY STONE_TABLEGEN_TARGETS ${CTG_TARGET})
  endif()
endfunction()

#===----------------------------------------------------------------------===//
# Windows Resource Versioning
#===----------------------------------------------------------------------===//

macro(set_stone_windows_version_resource_properties name)
  if(DEFINED windows_resource_file)
    set_windows_version_resource_properties(${name} ${windows_resource_file}
      VERSION_MAJOR ${STONE_VERSION_MAJOR}
      VERSION_MINOR ${STONE_VERSION_MINOR}
      VERSION_PATCHLEVEL ${STONE_VERSION_PATCHLEVEL}
      VERSION_STRING "${STONE_VERSION}"
      PRODUCT_NAME "stone")
  endif()
endmacro()

#===----------------------------------------------------------------------===//
# Subdirectory + Library Helpers
#===----------------------------------------------------------------------===//

macro(add_stone_subdirectory name)
  add_llvm_subdirectory(STONE TOOL ${name})
endmacro()

macro(add_stone_library name)
  cmake_parse_arguments(ARG "SHARED" "STATIC" "ADDITIONAL_HEADERS" ${ARGN})
  set(srcs)

  if(MSVC_IDE OR XCODE)
    file(RELATIVE_PATH lib_path ${STONE_SOURCE_DIR}/lib/ ${CMAKE_CURRENT_SOURCE_DIR})
    if(NOT lib_path MATCHES "^[.][.]")
      file(GLOB_RECURSE headers
        ${STONE_SOURCE_DIR}/include/stone/${lib_path}/*.h
        ${STONE_SOURCE_DIR}/include/stone/${lib_path}/*.def)
      file(GLOB_RECURSE tds
        ${STONE_SOURCE_DIR}/include/stone/${lib_path}/*.td)
      set_source_files_properties(${headers} ${tds} PROPERTIES HEADER_FILE_ONLY ON)
      source_group("TableGen descriptions" FILES ${tds})
      set(srcs ${headers} ${tds})
    endif()
  endif()

  if(srcs OR ARG_ADDITIONAL_HEADERS)
    set(srcs ADDITIONAL_HEADERS ${srcs} ${ARG_ADDITIONAL_HEADERS})
  endif()

  # Determine library type
  if(ARG_SHARED AND ARG_STATIC)
    set(LIBTYPE SHARED STATIC)
  elseif(ARG_SHARED)
    set(LIBTYPE SHARED)
  else()
    if(BUILD_SHARED_LIBS)
      set(LIBTYPE SHARED)
    else()
      set(LIBTYPE STATIC)
    endif()
    if(NOT XCODE AND NOT MSVC_IDE)
      list(APPEND LIBTYPE OBJECT)
    endif()
    set_property(GLOBAL APPEND PROPERTY STONE_STATIC_LIBS ${name})
  endif()

  llvm_add_library(${name} ${LIBTYPE} ${ARG_UNPARSED_ARGUMENTS} ${srcs})

  set(libs ${name})
  if(ARG_SHARED AND ARG_STATIC)
    list(APPEND libs ${name}_static)
  endif()

  foreach(lib ${libs})
    if(TARGET ${lib})
      target_link_libraries(${lib} INTERFACE ${LLVM_COMMON_LIBS})
      if(NOT LLVM_INSTALL_TOOLCHAIN_ONLY OR ARG_INSTALL_WITH_TOOLCHAIN)
        get_target_export_arg(${name} Stone export_to_stonetargets UMBRELLA stone-libraries)
        install(TARGETS ${lib}
          COMPONENT ${lib}
          ${export_to_stonetargets}
          LIBRARY DESTINATION lib${LLVM_LIBDIR_SUFFIX}
          ARCHIVE DESTINATION lib${LLVM_LIBDIR_SUFFIX}
          RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}")
        if(NOT LLVM_ENABLE_IDE)
          add_llvm_install_targets(install-${lib} DEPENDS ${lib} COMPONENT ${lib})
        endif()
        set_property(GLOBAL APPEND PROPERTY STONE_LIBS ${lib})
      endif()
      set_property(GLOBAL APPEND PROPERTY STONE_EXPORTS ${lib})
    else()
      add_custom_target(${lib})
    endif()
  endforeach()

  set_stone_windows_version_resource_properties(${name})
endmacro()

#===----------------------------------------------------------------------===//
# Executables & Tools
#===----------------------------------------------------------------------===//

macro(add_stone_executable name)
  add_llvm_executable(${name} ${ARGN})
  set_stone_windows_version_resource_properties(${name})
endmacro()

macro(add_stone_tool name)
  cmake_parse_arguments(ARG "DEPENDS;GENERATE_DRIVER" "" "" ${ARGN})

  if(NOT STONE_BUILD_TOOLS)
    set(EXCLUDE_FROM_ALL ON)
  endif()

  if(ARG_GENERATE_DRIVER AND LLVM_TOOL_LLVM_DRIVER_BUILD
     AND (NOT LLVM_DISTRIBUTION_COMPONENTS OR ${name} IN_LIST LLVM_DISTRIBUTION_COMPONENTS))
    list(FILTER ARGN EXCLUDE REGEX "^SUPPORT_PLUGINS$")
    generate_llvm_objects(${name} ${ARGN})
    add_custom_target(${name} DEPENDS llvm-driver stone-resource-headers)
  else()
    add_stone_executable(${name} ${ARGN})

    if(STONE_BUILD_TOOLS)
      get_target_export_arg(${name} Stone export_to_stonetargets)
      install(TARGETS ${name}
        ${export_to_stonetargets}
        RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
        COMPONENT ${name})

      if(NOT LLVM_ENABLE_IDE)
        add_llvm_install_targets(install-${name} DEPENDS ${name} COMPONENT ${name})
      endif()

      set_property(GLOBAL APPEND PROPERTY STONE_EXPORTS ${name})
    endif()
  endif()
endmacro()

#===----------------------------------------------------------------------===//
# Symlinks
#===----------------------------------------------------------------------===//

macro(add_stone_symlink name dest)
  get_property(LLVM_DRIVER_TOOLS GLOBAL PROPERTY LLVM_DRIVER_TOOLS)
  if(LLVM_TOOL_LLVM_DRIVER_BUILD AND ${dest} IN_LIST LLVM_DRIVER_TOOLS
     AND (NOT LLVM_DISTRIBUTION_COMPONENTS OR ${dest} IN_LIST LLVM_DISTRIBUTION_COMPONENTS))
    set_property(GLOBAL APPEND PROPERTY LLVM_DRIVER_TOOL_ALIASES_${dest} ${name})
  else()
    llvm_add_tool_symlink(STONE ${name} ${dest} ALWAYS_GENERATE)
    llvm_install_symlink(STONE ${name} ${dest} ALWAYS_GENERATE)
  endif()
endmacro()

#===----------------------------------------------------------------------===//
# Target Linking
#===----------------------------------------------------------------------===//

function(stone_target_link_libraries target type)
  if(TARGET obj.${target})
    target_link_libraries(obj.${target} ${ARGN})
  endif()

  get_property(LLVM_DRIVER_TOOLS GLOBAL PROPERTY LLVM_DRIVER_TOOLS)
  if(LLVM_TOOL_LLVM_DRIVER_BUILD AND ${target} IN_LIST LLVM_DRIVER_TOOLS)
    set(target llvm-driver)
  endif()

  if(STONE_LINK_STONE_DYLIB)
    target_link_libraries(${target} ${type} stone-cpp)
  else()
    target_link_libraries(${target} ${type} ${ARGN})
  endif()
endfunction()
