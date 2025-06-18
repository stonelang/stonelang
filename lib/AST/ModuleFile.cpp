#include "stone/AST/ModuleFile.h"
#include "stone/AST/Decl.h"

using namespace stone;

namespace stone {

ModuleFile::ModuleFile(unsigned bufferID, llvm::StringRef input, Module *parent)
    : FileArtifact(parent), kind(kind), bufferID(bufferID) {}

Decl *ModuleFile::GetFirstDecl() const { return nullptr; }

bool ModuleFile::HasFirstDecl() const { return GetFirstDecl() != nullptr; }

llvm::ArrayRef<Decl *> ModuleFile::GetTopLevelDecls() const {
  return topLevelDecls;
}

void ModuleFile::Flush() {}

// ModuleFile *ASTSession::CreateModuleFile(unsigned bufferID, llvm::StringRef
// input,
//                                    SpaceDecl *parent) {
//   return nullptr;
// }
