#ifndef STONE_AST_LOGIC_OBSERVER_H
#define STONE_AST_LOGIC_OBSERVER_H

namespace stone {
class LogicObserver {

public:
  LogicObserver() {}
  virtual ~LogicObserver() = default;

public:
  /// Completed semantic analysis
  virtual void CompletedDecl(const Decl *result) {}

  /// Completed semantic analysis
  virtual void CompletedSourceFile(const SourceFile *result) {}

  /// Completed semantic analysis
  virtual void CompletedSpaceDecl(const SpaceDecl *result) {}
};

} // namespace stone
#endif