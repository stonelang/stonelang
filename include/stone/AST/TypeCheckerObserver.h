#ifndef STONE_AST_TYPECHECKER_OBSERVER_H
#define STONE_AST_TYPECHECKER_OBSERVER_H

namespace stone {
class TypeCheckerObserver {

public:
  TypeCheckerObserver() {}
  virtual ~TypeCheckerObserver() = default;

public:
  /// Completed semantic analysis
  virtual void CompletedDecl(const Decl *result) {}

  /// Completed semantic analysis
  virtual void CompletedSourceFile(const SourceFile *result) {}

  /// Completed semantic analysis
  virtual void CompletedModuleDecl(const ModuleDecl *result) {}
};

} // namespace stone
#endif