#ifndef STONE_CODEGEN_CODEGENOBSERVER_H
#define STONE_CODEGEN_CODEGENOBSERVER_H

namespace stone {
class CodeGenObserver {

public:
  CodeGenObserver() {}
  virtual ~CodeGenObserver() = default;

public:
  /// Some executions may require access to the results of ir generation.
  virtual void CompletedModule(const llvm::Module *result) {}
};

} // namespace stone
#endif
