#ifndef STONE_AST_SCAFFOLDER_H
#define STONE_AST_SCAFFOLDER_H

namespace stone {
class ModuleDecl;

class Scaffolder final {

public:
  Scaffolder();

public:
  /// Check the module
  void ScaffoldModuleDecl(ModuleDecl &moduleDecl);
};

} // namespace stone

#endif