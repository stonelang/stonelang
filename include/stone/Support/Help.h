#ifndef STONE_SUPPORT_HELP_H
#define STONE_SUPPORT_HELP_H

#include "stone/Support/LangOptions.h"

namespace stone {
void PrintHelp(const LangOptions &langOpts, bool showHidden = false);
}
#endif