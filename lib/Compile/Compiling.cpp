#include "stone/Compile/Compiling.h"
#include "stone/Compile/Frontend.h"

Compiling::Compiling(std::unique_ptr<Frontend> frontend)
    : frontend(std::move(frontend)) {}