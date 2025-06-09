#ifndef STONE_OPTION_HELP_H
#define STONE_OPTION_HELP_H

#include "stone/Option/LangOptions.h"

namespace stone {
void PrintHelp(const LangOptions &langOpts, bool showHidden = false);
}
#endif