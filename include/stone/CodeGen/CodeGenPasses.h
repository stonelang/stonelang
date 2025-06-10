#ifndef STONE_CODEGEN_CODEGENPASSING_H
#define STONE_CODEGEN_CODEGENPASSING_H

#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/PassManager.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Target/TargetMachine.h"

#include <memory>

namespace stone {
class CodeGenPasses final {
  const CodeGenOptions &codeGenOpts;

  llvm::PassBuilder pb;
  llvm::LoopAnalysisManager lam;
  llvm::FunctionAnalysisManager fam;
  llvm::CGSCCAnalysisManager cgam;
  llvm::ModuleAnalysisManager mam;
  llvm::ModulePassManager mpm;
  llvm::legacy::PassManager lpm;
  std::unique_ptr<llvm::legacy::FunctionPassManager> lfpm;
  llvm::FunctionPassManager fpm;

public:
  CodeGenPasses(const CodeGenPasses &) = delete;
  void operator=(const CodeGenPasses &) = delete;
  CodeGenPasses(CodeGenPasses &&) = delete;
  void operator=(CodeGenPasses &&) = delete;

public:
  CodeGenPasses(const CodeGenOptions &codeGenOpts);

public:
  llvm::PassBuilder &GetPassBuilder() { return pb; }
  llvm::LoopAnalysisManager &GetLoopAnalysisManager() { return lam; }
  llvm::FunctionAnalysisManager &GetFunctionAnalysisManager() { return fam; }
  llvm::CGSCCAnalysisManager &GetCGSCCAnalysisManager() { return cgam; }
  llvm::ModuleAnalysisManager &GetModuleAnalysisManager() { return mam; }
  llvm::ModulePassManager &GetPassManager() { return mpm; }
  llvm::legacy::PassManager &GetLegacyPassManager() { return lpm; }

  bool HasLegacyFunctionPassManager() { return lfpm != nullptr; }

  llvm::legacy::FunctionPassManager &GetLegacyFunctionPassManager() {
    assert(HasLegacyFunctionPassManager() &&
           "Instance of LegacyFunctionPassManager not created!");
    return *lfpm;
  }
  llvm::FunctionPassManager &GetFunctionPassManager() { return fpm; }

public:
  const CodeGenOptions &GetCodGenOptions() const { return codeGenOpts; }

public:
  void RunPasses(llvm::Module *mod);
  void RunLegacyPasses(llvm::Module *mod);
};

} // namespace stone
#endif
