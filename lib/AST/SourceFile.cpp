#include "stone/AST/SourceFile.h"
#include "stone/AST/ModuleDecl.h"

using namespace stone;

SourceFile::SourceFile(SourceFileKind kind, unsigned srcBufferID,
                       ModuleDecl *parent)
    : ASTUnit(GetASTSession(parent), parent), kind(kind),
      srcBufferID(srcBufferID) {}

Decl *SourceFile::GetFirstDecl() const { return nullptr; }
bool SourceFile::HasFirstDecl() const { return GetFirstDecl() != nullptr; }
llvm::ArrayRef<Decl *> SourceFile::GetTopLevelDecls() const {
  return topLevelDecls;
}

ASTSession &SourceFile::GetASTSession(ModuleDecl *parent) {
  assert(parent && "Cannot get session from null ModuleDecl");
  return parent->GetASTSession();
}
