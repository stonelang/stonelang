#include "stone/AST/ModuleFile.h"
#include "stone/AST/ModuleDecl.h"

using namespace stone;

ModuleFile::ModuleFile(ModuleFileKind kind, ModuleDecl *parent)
    : ASTUnit(GetASTSession(parent), parent), kind(kind) {}

ASTSession &ModuleFile::GetASTSession(ModuleDecl *parent) {
  assert(parent && "Cannot get session from null ModuleDecl");
  return parent->GetASTSession();
}

SourceFile::SourceFile(SourceFileKind kind, unsigned srcBufferID,
                       ModuleDecl *parent)
    : ModuleFile(ModuleFileKind::Source, parent), kind(kind),
      srcBufferID(srcBufferID) {}

Decl *SourceFile::GetFirstDecl() const { return nullptr; }
bool SourceFile::HasFirstDecl() const { return GetFirstDecl() != nullptr; }
llvm::ArrayRef<Decl *> SourceFile::GetTopLevelDecls() const {
  return topLevelDecls;
}
