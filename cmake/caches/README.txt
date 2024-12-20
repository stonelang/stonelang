CMake Caches
============

This directory contains CMake cache scripts that pre-populate the CMakeCache in
a build directory with commonly used settings.

You can use the caches files with the following CMake invocation:

cmake -G <build system>
  -C <path to cache file>
  [additional CMake options (i.e. -DCMAKE_INSTALL_PREFIX=<install path>)]
  <path to llvm>

Options specified on the command line will override options in the cache files.

The following cache files exist.

Apple-stage1
------------

The Apple stage1 cache configures a two stage build similar to how Apple builds
the stone shipped with Xcode. The build files generated from this invocation has
a target named "stage2" which performs an LTO build of stone.

The Apple-stage2 cache can be used directly to match the build settings Apple
uses in shipping builds without doing a full bootstrap build.

PGO
---

The PGO CMake cache can be used to generate a multi-stage instrumented compiler.
You can configure your build directory with the following invocation of CMake:

cmake -G <generator> -C <path_to_stone>/cmake/caches/PGO.cmake <source dir>

After configuration the following additional targets will be generated:

stage2-instrumented:
Builds a stage1 x86 compiler, runtime, and required tools (llvm-config,
llvm-profdata) then uses that compiler to build an instrumented stage2 compiler.

stage2-instrumented-generate-profdata:
Depends on "stage2-instrumented" and will use the instrumented compiler to
generate profdata based on the training files in <stone>/utils/perf-training

stage2:
Depends on "stage2-instrumented-generate-profdata" and will use the stage1
compiler with the stage2 profdata to build a PGO-optimized compiler.

stage2-check-llvm:
Depends on stage2 and runs check-llvm using the stage3 compiler.

stage2-check-stone:
Depends on stage2 and runs check-stone using the stage3 compiler.

stage2-check-all:
Depends on stage2 and runs check-all using the stage3 compiler.

stage2-test-suite:
Depends on stage2 and runs the test-suite using the stage3 compiler (requires
in-tree test-suite).

3-stage
-------

This cache file can be used to generate a 3-stage stone build. You can configure
using the following CMake command:

cmake -C <path to stone>/cmake/caches/3-stage.cmake -G Ninja <path to llvm>

You can then run "ninja stage3-stone" to build stage1, stage2 and stage3 stones.

This is useful for finding non-determinism the compiler by verifying that stage2
and stage3 are identical.
