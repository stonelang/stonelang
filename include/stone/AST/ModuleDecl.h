#ifndef STONE_AST_MODULE_DECL_H
#define STONE_AST_MODULE_DECL_H

#include "stone/AST/AST.h"
#include "stone/AST/Decl.h"
#include "stone/AST/DeclContext.h"
#include "stone/AST/ModuleFile.h"

#include "llvm/ADT/ArrayRef.h"
#include "llvm/ADT/SmallVector.h"

namespace stone {
class ModuleFile;

class ModuleDecl final : public DeclContext,
                         public TypeDecl,
                         public ASTAllocation<ModuleDecl> {

  llvm::SmallVector<ModuleFile *, 2> moduleFiles;

public:
  ModuleDecl();
  explicit operator bool() const { return HasFirstModuleFile(); }

public:
  ModuleFile *GetFirstModuleFile() const;
  bool HasFirstModuleFile() const;

public:
  llvm::ArrayRef<ModuleFile *> GetModuleFiles() { return moduleFiles; }
  llvm::ArrayRef<const ModuleFile *> GetModuleFiles() const;
  void AddModuleFile(ModuleFile *moduleFile);
};

} // namespace stone

#endif