#ifndef STONE_LANG_ABOUT_H
#define STONE_LANG_ABOUT_H

#include "stone/Lang/Version.h"

namespace stone {
/// Print the current version of the compiler
void PrintCompilerVersion(const Version version);

/// Print the features the compiler supports
void PrintCompilerFeatures();

} // namespace stone
#endif