#include "stone/AST/File.h"
#include "stone/AST/Decl.h"
#include "stone/AST/Module.h"
#include "stone/AST/ModuleContext.h"
#include "stone/AST/Node.h"

using namespace stone;

// File::File(SrcUnit unit, Module &parent)
//     : Node(this), unit(unit), parent(parent) {}

Decl *File::GetFirstDecl() const { return nullptr; }

bool File::HasFirstDecl() const { return GetFirstDecl() != nullptr; }

llvm::ArrayRef<Decl *> File::GetTopLevelDecls() const { return topLevelDecls; }
