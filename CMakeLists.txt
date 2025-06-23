cmake_minimum_required(VERSION 3.20)

#===--------------------------------------------------------------------===#
# Project Configuration
#===--------------------------------------------------------------------===#

project(Stone)
set(LLVM_SUBPROJECT_TITLE "Stone")

# LLVM CMake Utilities (e.g., AddLLVM.cmake)
set(LLVM_COMMON_CMAKE_UTILS "${CMAKE_SOURCE_DIR}/../cmake" CACHE PATH "")
include(${LLVM_COMMON_CMAKE_UTILS}/Modules/CMakePolicy.cmake NO_POLICY_SCOPE)

#===--------------------------------------------------------------------===#
# CMake Module Path Setup
#===--------------------------------------------------------------------===#

list(PREPEND CMAKE_MODULE_PATH
  "${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules"
  "${LLVM_COMMON_CMAKE_UTILS}/Modules"
)

#===--------------------------------------------------------------------===#
# Standalone Mode Detection
#===--------------------------------------------------------------------===#

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
  set(STONE_BUILT_STANDALONE ON)
endif()

#===--------------------------------------------------------------------===#
# Prevent In-Source Builds
#===--------------------------------------------------------------------===#

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_BINARY_DIR AND NOT MSVC_IDE)
  message(FATAL_ERROR "In-source builds are not allowed. Please use a separate build directory.")
endif()

#===--------------------------------------------------------------------===#
# Directory Configuration
#===--------------------------------------------------------------------===#

set(STONE_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(STONE_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR})

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib${LLVM_LIBDIR_SUFFIX})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib${LLVM_LIBDIR_SUFFIX})

#===--------------------------------------------------------------------===#
# Language Configuration
#===--------------------------------------------------------------------===#

if(STONE_BUILT_STANDALONE)
  set(CMAKE_CXX_STANDARD 17)
  set(CMAKE_CXX_STANDARD_REQUIRED ON)
  set(CMAKE_CXX_EXTENSIONS OFF)
endif()

#===--------------------------------------------------------------------===#
# LLVM Integration
#===--------------------------------------------------------------------===#

if(STONE_BUILT_STANDALONE)
  find_package(LLVM REQUIRED HINTS "${LLVM_CMAKE_DIR}")
  list(APPEND CMAKE_MODULE_PATH "${LLVM_DIR}")

  include(AddLLVM)
  include(TableGen)
  include(HandleLLVMOptions)
  include(VersionFromVCS)
  include(CheckAtomic)
  include(GetErrcMessages)
  include(LLVMDistributionSupport)
endif()

#===--------------------------------------------------------------------===#
# Core Modules and Tools
#===--------------------------------------------------------------------===#

include(StoneLang)            # Macros and helpers
# include(StoneOptions)       # Optional feature toggles
# include(StoneVersion)       # Version management
# include(StoneDependencies)  # System checks and utilities
include(StoneFmt)             # Code formatting rules

#===--------------------------------------------------------------------===#
# Configuration Header
#===--------------------------------------------------------------------===#

configure_file(
  ${STONE_SOURCE_DIR}/include/stone/config.h.cmake
  ${STONE_BINARY_DIR}/include/stone/config.h
)

#===--------------------------------------------------------------------===#
# Global Include Paths
#===--------------------------------------------------------------------===#

include_directories(BEFORE
  ${STONE_SOURCE_DIR}/include
  ${STONE_BINARY_DIR}/include
  ${CMAKE_SOURCE_DIR}/tools/clang/include
  ${CMAKE_BINARY_DIR}/tools/clang/include
)

#===--------------------------------------------------------------------===#
# Subdirectories
#===--------------------------------------------------------------------===#

add_subdirectory(include)
add_subdirectory(lib)
add_subdirectory(tools)
add_subdirectory(cmake/modules)

#===--------------------------------------------------------------------===#
# Install Targets
#===--------------------------------------------------------------------===#

include(StoneInstall)

#===--------------------------------------------------------------------===#
# Optional Features
#===--------------------------------------------------------------------===#

if(STONE_INCLUDE_TESTS)
  # include(StoneTesting)
endif()

if(STONE_ENABLE_BOOTSTRAP)
  include(StoneBootstrap)
endif()

if(STONE_BOLT)
  include(StoneBOLT)
endif()

if(STONE_INCLUDE_DOCS)
  # add_subdirectory(docs)
endif()
