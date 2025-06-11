#ifndef STONE_AST_ASTUNIT_H
#define STONE_AST_ASTUNIT_H

#include "stone/AST/ASTAllocation.h"

#include <stdint.h>

namespace stone {

enum class ASTUnitKind : uint8_t {
  None = 0,
#define AST_UNIT(K) K,
#include "ASTUnit.def"
};

class ASTUnit : public ASTAllocation<ASTUnit> {
  ASTUnit *parent = nullptr;
  ASTSession &session;

public:
  ASTUnit(ASTSession &session, ASTUnit *parent = nullptr)
      : session(session), kind(kind) {}
  virtual ~ASTUnit() = default;

public:
  // void Walk(ASTWalker &walker);
  // void Walk(ASTWalker &&walker) { Walk(walker); }
  // void Visit(ASTVisitor& visitor);

  virtual ASTUnitKind GetUnitKind() const = 0;

  ASTUnit *GetParent() const { return parent; }
  ASTSession &GetSession() const { return session; }

public:
  // static bool classof(const ASTUnit *unit) {
  //   return unit->getKind() == ASTKind::Decl;
  // }
};

} // namespace stone

#endif
