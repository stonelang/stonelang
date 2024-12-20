//===--- Options.h - Option info & table ------------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_STONE_DRIVER_OPTIONS_H
#define LLVM_STONE_DRIVER_OPTIONS_H

#include "llvm/Option/OptTable.h"
#include "llvm/Option/Option.h"

namespace stone {
namespace opts {
enum StoneFlags {
    FrontendOption = (1 << 4),
    NoDriverOption = (1 << 5),
    NoInteractiveOption = (1 << 6),
    DoesNotAffectIncrementalBuild = (1 << 8),
    AutolinkExtractOption = (1 << 9),
    ModuleWrapOption = (1 << 10),
    StoneIndentOption = (1 << 11),
    ArgumentIsPath = (1 << 12),
    ModuleInterfaceOption = (1 << 13),
    SupplementaryOutput = (1 << 14),
    StoneAPIExtractOption = (1 << 15),
    StoneSymbolGraphExtractOption = (1 << 16),
    StoneAPIDigesterOption = (1 << 17),
    ModuleInterfaceOptionIgnorable = (1 << 18),
    ModuleInterfaceOptionIgnorablePrivate = (1 << 19),
    ArgumentIsFileList = (1 << 20),
    CacheInvariant = (1 << 21),
  };
enum ID {
    OPT_INVALID = 0, // This is not an option ID.
#define OPTION(...) LLVM_MAKE_OPT_ID(__VA_ARGS__),
#include "stone/Option/Options.inc"
    LastOption
#undef OPTION
  };
}
std::unique_ptr<llvm::opt::OptTable> CreateStoneOptTable();
}

#endif
