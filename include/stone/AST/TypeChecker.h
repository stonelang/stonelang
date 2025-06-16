#ifndef STONE_AST_TYPECHECKER_H
#define STONE_AST_TYPECHECKER_H

namespace stone {
class Decl;
class SourceFile;
class SpaceDecl;
class Expr;
class Stmt;
class Type;
class ValueDecl;

class SpaceDecl;

class TypeChecker final {
public:
  TypeChecker();

public:
  void CheckSpaceDecl(SpaceDecl &moduleDecl);
  /// Check the module

  /// Check the source file
  void CheckSourceFile(SourceFile &SF);

  /// Check the declaration
  void CheckDecl(Decl *D);
};

} // namespace stone

#endif