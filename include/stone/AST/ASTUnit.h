#ifndef STONE_AST_ASTUNIT_H
#define STONE_AST_ASTUNIT_H

#include "stone/AST/ASTAllocation.h"
#include "stone/AST/Identifier.h"

#include <stdint.h>

namespace stone {

enum class ASTUnitKind : uint8_t {
  None = 0,
#define AST_UNIT(K) K,
#include "ASTUnit.def"
};

class ASTUnit : public ASTAllocation<ASTUnit> {
  ASTUnit *parent = nullptr;

public:
  ASTUnit(ASTUnit *parent = nullptr);
  // virtual ~ASTUnit() = default;

public:
  // void Walk(ASTWalker &walker);
  // void Walk(ASTWalker &&walker) { Walk(walker); }
  // void Visit(ASTVisitor& visitor);

  virtual ASTUnitKind GetUnitKind() const = 0;
  bool HasParent() const { return parent != nullptr; }
  ASTUnit *GetParent() { return parent; }

  // virtual void Evaluate(EvaluatorKind kind);

public:
  bool IsDecl() const { return GetUnitKind() == ASTUnitKind::Decl; }
  bool IsExpr() const { return GetUnitKind() == ASTUnitKind::Expr; }
  bool IsStmt() const { return GetUnitKind() == ASTUnitKind::Stmt; }
  bool IsType() const { return GetUnitKind() == ASTUnitKind::Type; }
  bool IsModuleFile() const { return GetUnitKind() == ASTUnitKind::ModuleFile; }
  bool IsModule() const { return GetUnitKind() == ASTUnitKind::Module; }

public:
  // static bool classof(const ASTUnit *unit) {
  //   return unit->getKind() == ASTKind::Decl;
  // }
};

} // namespace stone

#endif
