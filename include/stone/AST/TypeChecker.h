#ifndef STONE_AST_TYPECHECKER_H
#define STONE_AST_TYPECHECKER_H

#include "stone/AST/ModuleDecl.h"

namespace stone {
class Decl;
class SourceFile;
class ModuleDecl;
class Expr;
class Stmt;
class Type;
class ValueDecl;

/// Check the module
void CheckModuleDecl(ModuleDecl &MD);

/// Check the source file
void CheckSourceFile(SourceFile &SF);

/// Check the declaration
void CheckDecl(Decl *D);

} // namespace stone

#endif