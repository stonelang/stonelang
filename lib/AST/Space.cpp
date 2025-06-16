#include "stone/AST/Space.h"
#include "stone/AST/ASTFile.h"
#include "stone/AST/ASTSession.h"

using namespace stone;

SpaceDecl::SpaceDecl(DeclState *DS) : TypeDecl(DS) {
  // scope = new (session) ASTScope(this);
}

ASTFile *SpaceDecl::GetFirstASTFile() const { return nullptr; }

bool SpaceDecl::HasFirstASTFile() const { return GetFirstASTFile() != nullptr; }

llvm::ArrayRef<const ASTFile *> SpaceDecl::GetASTFiles() const {
  return {files.begin(), files.size()};
}

void SpaceDecl::AddASTFile(ASTFile *file) {
  assert(file && "Cannot add null ASTFile");
  files.push_back(file);
}

NormalSpaceDecl *ASTSession::CreateNormalSpaceDecl() {
  // return new (*this) NormalSpaceDecl(*this);
}

BuiltinSpaceDecl *ASTSession::CreateBuiltinSpaceDecl() {
  // return new (*this) BuiltinSpaceDecl(*this);
}

ForeignSpaceDecl *
ASTSession::CreateForeignSpaceDecl(ForeignSpaceDeclKind kind) {
  // new (*this) ForeignSpaceDecl(kind, *this);
}
