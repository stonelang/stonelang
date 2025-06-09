#include "stone/AST/ModuleFile.h"
#include "stone/AST/ModuleDecl.h"

using namespace stone;

ModuleFile::ModuleFile(ModuleFileKind kind, ModuleDecl *owner)
    : DeclContext(owner), kind(kind) {}

Decl *SourceFile::GetFirstDecl() const { return nullptr; }

bool SourceFile::HasFirstDecl() const { return GetFirstDecl() != nullptr; }

llvm::ArrayRef<Decl *> SourceFile::GetTopLevelDecls() const {
  return topLevelDecls;
}
