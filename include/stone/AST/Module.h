#ifndef STONE_AST_MODULE_H
#define STONE_AST_MODULE_H

#include "stone/AST/ASTMemory.h"
#include "stone/AST/FileUnit.h"

#include "llvm/ADT/SmallVector.h"

namespace stone {
class Module;
class ASTScope;

class Module final : public FileUnit {
  ASTMemory &memory;
  ASTScope *scope = nullptr;
  llvm::SmallVector<ModuleFile *, 16> files;
  llvm::SmallVector<const Module *> deps;
  IdentifierTable identifiers;

public:
  Module(const Module &) = delete;
  void operator=(const Module &) = delete;

public:
  explicit Module(ASTMemory &memory) : ASTUnit(memory);
  explicit operator bool() const { return HasFiles(); }

public:
  // Declare the set of builtin identifiers.
#define BUILTIN_TYPE(ID, Parent) const Type *Builtin##ID##Type;
#include "stone/AST/TypeNode.def"

  // Declare the set of builtin identifiers.
#define BUILTIN_IDENTIFIER_WITH_NAME(Name, IdStr)                              \
  Identifier Builtin##Name##Identifier;
#include "stone/AST/BuiltinIdentifiers.def"
  Identifier GetIdentifier(llvm::StringRef text) const;

public:
  llvm::ArrayRef<ModuleFile *> GetFiles() const { return files; }
  void AddFile(ModuleFile *file) { files.push_back(file); }
  ASTUnitKind GetUnitKind() const override { return ASTUnitKind::Module; }
  void Flush() override;

  ModuleFile *GetFirstFile() const;
  bool HasFiles() const { return !files.empty(); }

  ASTScope *GetScope() const { return scope; }
  void SetScope(ASTScope *S) { scope = S; }

  llvm::ArrayRef<const Module *> GetDeps() const { return deps; }
  void AddDep(const Module *S) { deps.push_back(S); }

  Identifier GetIdentifier(llvm::StringRef text) const;
  ASTMemory &GetMemory() { return memory; }
};
} // namespace stone

#endif
