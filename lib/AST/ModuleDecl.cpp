#include "stone/AST/ModuleDecl.h"
#include "stone/AST/ASTSession.h"
#include "stone/AST/SourceFile.h"

using namespace stone;

ModuleDecl::ModuleDecl(DeclState *DS) : TypeDecl(DS) {
  // scope = new (session) ASTScope(this);
}

SourceFile *ModuleDecl::GetFirstSourceFile() const { return nullptr; }

bool ModuleDecl::HasFirstSourceFile() const {
  return GetFirstSourceFile() != nullptr;
}

llvm::ArrayRef<const SourceFile *> ModuleDecl::GetSourceFiles() const {
  return {sources.begin(), sources.size()};
}

void ModuleDecl::AddSourceFile(SourceFile *sourceFile) {
  assert(sourceFile && "Cannot add null SourceFile");
  sources.push_back(sourceFile);
}

NormalModuleDecl *ASTSession::CreateNormalModuleDecl() {
  // return new (*this) NormalModuleDecl(*this);
}

BuiltinModuleDecl *ASTSession::CreateBuiltinModuleDecl() {
  // return new (*this) BuiltinModuleDecl(*this);
}

ForeignModuleDecl *
ASTSession::CreateForeignModuleDecl(ForeignModuleDeclKind kind) {
  // new (*this) ForeignModuleDecl(kind, *this);
}
