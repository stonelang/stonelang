#include "stone/CodeGen/CodeGenPasses.h"

using namespace stone;

CodeGenPasses::CodeGenPasses(const CodeGenOptions &codeGenOpts)
    : codeGenOpts(codeGenOpts) {
  pb.registerModuleAnalyses(mam);
  pb.registerCGSCCAnalyses(cgam);
  pb.registerFunctionAnalyses(fam);
  pb.registerLoopAnalyses(lam);
  pb.crossRegisterProxies(lam, fam, cgam, mam);
  mpm = pb.buildPerModuleDefaultPipeline(codeGenOpts.GetOptimizationLevel());
}

void CodeGenPasses::RunPasses(llvm::Module *mod) {
  GetPassManager().run(*mod, GetModuleAnalysisManager());
}

void CodeGenPasses::RunLegacyPasses(llvm::Module *mod) {
  lfpm = std::make_unique<llvm::legacy::FunctionPassManager>(mod);
  GetLegacyPassManager().run(*mod);
}
