#ifndef STONE_AST_MODULE_H
#define STONE_AST_MODULE_H

#include "stone/AST/FileArtifact.h"
#include "stone/AST/MemoryManager.h"

#include "llvm/ADT/SmallVector.h"

namespace stone {
class Module;
class Scope;

class Module final : public FileArtifact {
  ModuleContext &context;
  Scope *scope = nullptr;
  llvm::SmallVector<ModuleFile *, 16> files;
  llvm::SmallVector<const Module *> deps;

public:
  Module(const Module &) = delete;
  void operator=(const Module &) = delete;

public:
  explicit Module(ModuleContext &context);
  explicit operator bool() const { return HasFiles(); }

public:
  llvm::ArrayRef<ModuleFile *> GetFiles() const { return files; }
  void AddFile(ModuleFile *file) { files.push_back(file); }
  ArtifactKind GetUnitKind() const override { return ArtifactKind::Module; }
  void Flush() override;

  ModuleFile *GetFirstFile() const;
  bool HasFiles() const { return !files.empty(); }

  Scope *GetScope() const { return scope; }
  void SetScope(Scope *S) { scope = S; }

  llvm::ArrayRef<const Module *> GetDeps() const { return deps; }
  void AddDep(const Module *S) { deps.push_back(S); }

  Identifier GetIdentifier(llvm::StringRef text) const;
  MemoryManager &GetMemory() { return memory; }
};

} // namespace stone

#endif
