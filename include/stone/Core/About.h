#ifndef STONE_CORE_ABOUT_H
#define STONE_CORE_ABOUT_H

#include "stone/Core/Features.h"
#include "stone/Core/Version.h"

namespace stone {
/// Print the current version of the compiler
void PrintCompilerVersion(const Version version);

/// Print the features the compiler supports
void PrintCompilerFeatures(const Features features);

} // namespace stone
#endif