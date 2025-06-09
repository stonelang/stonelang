#ifndef STONE_AST_MODULE_DECL_H
#define STONE_AST_MODULE_DECL_H

#include "stone/AST/AST.h"
#include "stone/AST/Decl.h"
#include "stone/AST/DeclContext.h"
#include "stone/AST/ModuleFile.h"

#include "llvm/ADT/SmallVector.h"

namespace stone {

class ModuleDecl final : public DeclContext,
                         public TypeDecl,
                         public ASTAllocation<ModuleDecl> {
public:
  ModuleDecl();

  explicit operator bool() const { return HasFirstModuleFile(); }

public:
  ModuleFile *GetFirstModuleFile() const;
  bool HasFirstModuleFile() const;

public:
};

} // namespace stone

#endif