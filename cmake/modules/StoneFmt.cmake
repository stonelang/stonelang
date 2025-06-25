#===--------------------------------------------------------------------===#
# Stone Format Targets
#===--------------------------------------------------------------------===#

# Globs for include headers (grouped by subdir, merged into one variable)
file(GLOB_RECURSE stone_include_files
  #${STONE_SOURCE_DIR}/include/stone/*.h
  ${STONE_SOURCE_DIR}/include/stone/AST/*.h
  ${STONE_SOURCE_DIR}/include/stone/CodeGen/*.h
  ${STONE_SOURCE_DIR}/include/stone/Compile/*.h
  ${STONE_SOURCE_DIR}/include/stone/Core/*.h
  ${STONE_SOURCE_DIR}/include/stone/Diag/*.h
  ${STONE_SOURCE_DIR}/include/stone/Driver/*.h
  ${STONE_SOURCE_DIR}/include/stone/Lex/*.h
  ${STONE_SOURCE_DIR}/include/stone/Parse/*.h
  
)

# Globs for library sources (grouped by subdir, merged into one variable)
file(GLOB_RECURSE stone_lib_files
  ${STONE_SOURCE_DIR}/lib/AST/*.cpp
  ${STONE_SOURCE_DIR}/lib/CodeGen/*.cpp
  ${STONE_SOURCE_DIR}/lib/Compile/*.cpp
  ${STONE_SOURCE_DIR}/lib/Driver/*.cpp
  ${STONE_SOURCE_DIR}/lib/Diag/*.cpp
  ${STONE_SOURCE_DIR}/lib/Lex/*.cpp
  ${STONE_SOURCE_DIR}/lib/Parse/*.cpp
  ${STONE_SOURCE_DIR}/lib/Core/*.cpp
)

# Globs for tool sources (optional)
file(GLOB_RECURSE stone_tools_files
  ${STONE_SOURCE_DIR}/tools/stonec/*.cpp
  ${STONE_SOURCE_DIR}/tools/stonef/*.cpp
)

# Run clang-format on everything if available
find_program(CLANG_FORMAT clang-format)

if(CLANG_FORMAT)
  add_custom_target(stone-format
    COMMAND ${CLANG_FORMAT} -i -style=llvm
      ${stone_include_files}
      ${stone_lib_files}
      ${stone_tools_files}
    VERBATIM
    COMMENT "Formatting all Stone source files with clang-format"
  )
else()
  message(STATUS "clang-format not found. Skipping stone-format target.")
endif()
