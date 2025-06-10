#ifndef STONE_CODEGEN_CODEGENERATION_H
#define STONE_CODEGEN_CODEGENERATION_H

#include "stone/Lang/CodeGenOptions.h"

#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/PassManager.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Target/TargetMachine.h"

namespace stone {

class CodeGeneration final {
public:
  CodeGeneration();
};

} // namespace stone
#endif
