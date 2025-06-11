#include "stone/AST/ModuleDecl.h"
#include "stone/AST/ASTBuilder.h"
#include "stone/AST/ASTSession.h"
#include "stone/AST/ModuleFile.h"

using namespace stone;

ModuleDecl::ModuleDecl(ASTSession &session)
    : TypeDecl(DeclKind::Module, session) {}

ModuleFile *ModuleDecl::GetFirstModuleFile() const { return nullptr; }

bool ModuleDecl::HasFirstModuleFile() const {
  return GetFirstModuleFile() != nullptr;
}

llvm::ArrayRef<const ModuleFile *> ModuleDecl::GetModuleFiles() const {
  return {moduleFiles.begin(), moduleFiles.size()};
}

void ModuleDecl::AddModuleFile(ModuleFile *moduleFile) {
  // If this is a LoadedFile, make sure it loaded without error.
  // assert(!(isa<LoadedFile>(newFile) &&
  //         cast<LoadedFile>(newFile).hadLoadError()));
  // Require Main and REPL files to be the first file added.
  // assert(files.empty() || !isa<SourceFile>(file) ||
  //        llvm::cast<SourceFile>(file).kind == SourceFileKind::Library
  /// ||cast<SourceFile>(unit).Kind == SourceFileKind::SIL);
  moduleFiles.push_back(moduleFile);
  // TODO: ClearLookupCache();
}

ModuleDecl *ASTBuilder::CreateModuleDecl() { return nullptr; }
