#ifndef STONE_AST_MODULE_DECL_H
#define STONE_AST_MODULE_DECL_H

#include "stone/AST/ASTSession.h"
#include "stone/AST/Decl.h"
#include "stone/AST/ModuleFile.h"

#include "llvm/ADT/ArrayRef.h"
#include "llvm/ADT/SmallVector.h"

namespace stone {
class ModuleFile;
class ASTSession;

class ModuleDecl final : public TypeDecl {

  llvm::SmallVector<ModuleFile *, 2> moduleFiles;

public:
  ModuleDecl(ASTSession &session);
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