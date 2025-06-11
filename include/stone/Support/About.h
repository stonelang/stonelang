#ifndef STONE_SUPPORT_ABOUT_H
#define STONE_SUPPORT_ABOUT_H

#include "stone/Support/Features.h"
#include "stone/Support/Version.h"

namespace stone {
/// Print the current version of the compiler
void PrintCompilerVersion(const Version version);

/// Print the features the compiler supports
void PrintCompilerFeatures(const Features features);

} // namespace stone
#endif