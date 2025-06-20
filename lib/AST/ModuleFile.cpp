#include "stone/AST/ModuleFile.h"
#include "stone/AST/Decl.h"
#include "stone/AST/Module.h"

using namespace stone;

ModuleFile::ModuleFile(unsigned bufferID, llvm::StringRef input, Module *parent)
    : Node(parent), bufferID(bufferID) {}

Decl *ModuleFile::GetFirstDecl() const { return nullptr; }

bool ModuleFile::HasFirstDecl() const { return GetFirstDecl() != nullptr; }

llvm::ArrayRef<Decl *> ModuleFile::GetTopLevelDecls() const {
  return topLevelDecls;
}

// void ModuleFile::Flush() {}

// ModuleFile *ASTSession::CreateModuleFile(unsigned bufferID, llvm::StringRef
// input,
//                                    SpaceDecl *parent) {
//   return nullptr;
// }
