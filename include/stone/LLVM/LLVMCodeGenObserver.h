#ifndef STONE_CODEGEN_CODEGENOBSERVER_H
#define STONE_CODEGEN_CODEGENOBSERVER_H

namespace stone {
struct LLVMCodeGenObserver {
  virtual void DidStartCodeGen(stone::Module *m) {}
  virtual void DidEmitFunction(stone::FunDecl *func) {}
  virtual void DidFinishCodeGen(llvm::Module *result) {}
};

} // namespace stone
#endif
