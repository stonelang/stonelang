#ifndef STONE_LANG_HELP_H
#define STONE_LANG_HELP_H

#include "stone/Lang/LangOptions.h"

namespace stone {
void PrintHelp(const LangOptions &langOpts, bool showHidden = false);
}
#endif