#include "stone/Support/LangOptions.h"

LangOptions::LangOptions()
    : optTable(stone::CreateOptTable()),
      DefaultTargetTriple(llvm::sys::getDefaultTargetTriple()) {

  llvm::sys::fs::current_path(WorkingDirectory);
}

bool LangOptions::HasTargetTriple(llvm::Triple triple) {
  switch (triple.getOS()) {
  case llvm::Triple::Darwin:
  case llvm::Triple::MacOSX:
  case llvm::Triple::Linux:
  case llvm::Triple::FreeBSD:
  case llvm::Triple::OpenBSD:
  case llvm::Triple::Win32:
  case llvm::Triple::Haiku:
    return true;
  case llvm::Triple::UnknownOS:
    return false;
  }
  llvm_unreachable("Unknown Triple");
}

/// Print out the options
void LangOptions::PrintHelp(bool showHidden) {}