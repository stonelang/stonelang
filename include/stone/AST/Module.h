#ifndef STONE_AST_MODULE_H
#define STONE_AST_MODULE_H

#include "stone/AST/AST.h"
#include "stone/AST/DeclContext.h"

#include "llvm/ADT/SmallVector.h"

namespace stone {

static inline unsigned AlignOfModuleFile();

class ModuleDecl;
class SourceFile;
class ModuleFile;

enum class ModuleFileKind : uint8_t { Syntax, Builtin };

class ModuleFile : public DeclContext, public ASTAllocation<ModuleFile> {
private:
  ModuleFileKind kind;

public:
  ModuleFile(ModuleFileKind kind, ModuleDecl *owner);
};

class SourceFile final : public ModuleFile {
public:
};

class ModuleDecl
    : public DeclContext, public TypeDecl, public ASTAllocation<ModuleDecl> 


};

} // namespace stone

#endif