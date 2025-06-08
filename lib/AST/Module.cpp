#include "stone/AST/Module.h"

using namespace stone;

ModuleFile::ModuleFile(ModuleFileKind kind, ModuleDecl *owner)
    : DeclContext(owner), kind(kind) {}
