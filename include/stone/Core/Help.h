#ifndef STONE_CORE_HELP_H
#define STONE_CORE_HELP_H

#include "stone/Core/LangOptions.h"

namespace stone {
void PrintHelp(const LangOptions &langOpts, bool showHidden = false);
}
#endif