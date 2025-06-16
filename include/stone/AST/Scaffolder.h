#ifndef STONE_AST_SCAFFOLDER_H
#define STONE_AST_SCAFFOLDER_H

namespace stone {
class Decl;
class SpaceDecl;

class Scaffolder final {

public:
  Scaffolder();

public:
  /// Produce symbols and resolve names
  void ScaffoldDecl(Decl *D);

  /// Produce symbols and resolve names
  void ScaffoldSpaceDecl(SpaceDecl *MD);
};

} // namespace stone

#endif