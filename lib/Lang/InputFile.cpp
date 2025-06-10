#include "stone/Lang/InputFile.h"

#include "llvm/ADT/StringRef.h"

using namespace stone;

llvm::StringRef
stone::ConvertBufferNameFromLLVMGetFileOrSTDINToStoneConventions(
    llvm::StringRef filename) {
  if (filename.equals("<stdin>")) {
    return "-";
  }
  return filename;
}