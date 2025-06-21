#include "stone/AST/ModuleFile.h"
#include "stone/AST/Decl.h"
#include "stone/AST/Module.h"
#include "stone/AST/ModuleContext.h"

using namespace stone;

ModuleFile::ModuleFile(unsigned bufferID, llvm::StringRef input, Module &parent)
    : Node(&parent), bufferID(bufferID), input(input),
      DE(parent.GetContext().GetDiags()) {}

Decl *ModuleFile::GetFirstDecl() const { return nullptr; }

bool ModuleFile::HasFirstDecl() const { return GetFirstDecl() != nullptr; }

llvm::ArrayRef<Decl *> ModuleFile::GetTopLevelDecls() const {
  return topLevelDecls;
}
