#include "stone/AST/ModuleDecl.h"
#include "stone/AST/ASTSession.h"
#include "stone/AST/SourceFile.h"

using namespace stone;

ModuleDecl::ModuleDecl(ModuleDeclKind kind, ASTSession &session)
    : TypeDecl(DeclKind::Module, session), kind(kind) {
  scope = new (session) ASTScope(this);
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
