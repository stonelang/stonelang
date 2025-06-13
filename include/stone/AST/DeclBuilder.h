#ifndef STONE_AST_DECL_BUILDER_H
#define STONE_AST_DECL_BUILDER_H

#include <stdint.h>

namespace stone {
class ModuleDecl;
class JoinDecl;
class SpaceDecl;
class UsingDecl;
class FunDecl;
class StructDecl;
class InterfaceDecl;
class EnumDecl;
class NormalModuleDecl;
class BuiltinModuleDecl;
class ForeignModuleDecl;
class ASTSession;

class DeclBuilder final {
  ASTSession &session;

public:
  DeclBuilder(ASTSession &session);

public:
  JoinDecl *BuildJoinDecl();
  SpaceDecl *BuildSpaceDecl();
  UsingDecl *BuildUsingDecl();

public:
  NormalModuleDecl *BuildNormalModuleDecl();
  BuiltinModuleDecl *BuildBuiltinModuleDecl();
  ForeignModuleDecl *BuildForeignModuleDecl();

public:
  FunDecl *BuildFunDecl();

public:
};

} // namespace stone

#endif
