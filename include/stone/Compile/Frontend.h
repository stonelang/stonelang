#ifndef STONE_COMPILE_FRONTEND_H
#define STONE_COMPILE_FRONTEND_H

#include "stone/AST/TypeCheckerOptions.h"
#include "stone/Compile/FrontendOptions.h"
#include "stone/Lang/CodeGenOptions.h"
#include "stone/Lang/DiagnosticOptions.h"
#include "stone/Lang/LangOptions.h"
#include "stone/Lang/Status.h"

#include "clang/Basic/FileManager.h"
#include "clang/Basic/FileSystemOptions.h"

#include "llvm/ADT/ArrayRef.h"
#include "llvm/ADT/SmallVector.h"
#include "llvm/Option/ArgList.h"
#include <memory>

namespace stone {

using ConfigurationFileBuffers =
    llvm::SmallVector<std::unique_ptr<llvm::MemoryBuffer>, 4>;

struct ModuleBuffers final {
  std::unique_ptr<llvm::MemoryBuffer> moduleBuffer;
  std::unique_ptr<llvm::MemoryBuffer> moduleDocBuffer;
  std::unique_ptr<llvm::MemoryBuffer> moduleSourceInfoBuffer;
  // Constructor
  explicit ModuleBuffers(
      std::unique_ptr<llvm::MemoryBuffer> moduleBuffer,
      std::unique_ptr<llvm::MemoryBuffer> moduleDocBuffer = nullptr,
      std::unique_ptr<llvm::MemoryBuffer> moduleSourceInfoBuffer = nullptr)
      : moduleBuffer(std::move(moduleBuffer)),
        moduleDocBuffer(std::move(moduleDocBuffer)),
        moduleSourceInfoBuffer(std::move(moduleSourceInfoBuffer)) {}
};

using MemoryBuffers =
    llvm::SmallVectorImpl<std::unique_ptr<llvm::MemoryBuffer>>;

class Frontend final {
  FrontendOptions frontendOpts;
  LangOptions langOpts;
  CodeGenOptions codeGenOpts;
  TypeCheckerOptions typeCheckerOpts;
  DiagnosticOptions diagOpts;
  clang::FileSystemOptions clangFileSystemOpts;
  std::unique_ptr<llvm::opt::InputArgList> inputArgList;

public:
  Frontend();
  Status ParseArgStrings(llvm::ArrayRef<const char *> args);

public:
  FrontendOptions &GetFronendOptions() { return frontendOpts; }
  LangOptions &GetLangOptions() { return langOpts; }
  CodeGenOptions &GetCodeGenOptions() { return codeGenOpts; }
  DiagnosticOptions &GetDiagnosticOptions() { return diagOpts; }
  clang::FileSystemOptions &GetClangFileSystemOptions() {
    return clangFileSystemOpts;
  }
};

} // namespace stone

#endif // STONE_FRONTEND_FRONTENDINPUTFILE_H
