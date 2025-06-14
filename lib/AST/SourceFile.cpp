#include "stone/AST/SourceFile.h"
#include "stone/AST/ASTSession.h"
#include "stone/AST/ModuleDecl.h"

using namespace stone;

namespace stone {
static ASTSession &GetASTSession(ModuleDecl *parent) {
  assert(parent && "Cannot get session from null ModuleDecl");
  return parent->GetASTSession();
}
} // namespace stone

SourceFile::SourceFile(SourceFileKind kind, unsigned srcBufferID,
                       ModuleDecl *parent)
    : ASTUnit(stone::GetASTSession(parent), parent), kind(kind),
      srcBufferID(srcBufferID) {}

Decl *SourceFile::GetFirstDecl() const { return nullptr; }
bool SourceFile::HasFirstDecl() const { return GetFirstDecl() != nullptr; }
llvm::ArrayRef<Decl *> SourceFile::GetTopLevelDecls() const {
  return topLevelDecls;
}

SourceFile *ASTSession::CreateSourceFile(SourceFileKind kind,
                                         unsigned srcBufferID,
                                         ModuleDecl *parent) {
  return nullptr;
}
