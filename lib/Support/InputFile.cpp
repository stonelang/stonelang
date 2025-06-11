#include "stone/Support/InputFile.h"

using namespace stone;

llvm::StringRef
stone::ConvertBufferNameFromLLVMGetFileOrSTDINToStoneConventions(
    llvm::StringRef filename) {
  if (filename.equals("<stdin>")) {
    return "-";
  }
  return filename;
}