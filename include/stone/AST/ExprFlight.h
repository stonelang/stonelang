#ifndef STONE_AST_EXPRFLIGHT_H
#define STONE_AST_EXPRFLIGHT_H

#include "stone/AST/Alignments.h"
#include "stone/AST/Allocation.h"
#include "stone/AST/ExprKind.h"
#include "stone/AST/TypeInfluencer.h"

namespace stone {

class alignas(8) ExprFlight : public Allocation<ExprFlight> {
  Expr *owner;
  ExprFlight *parent;

public:
  ExprFlight(ExprFlight *parent = nullptr) : parent(parent) {}
  Expr *GetOwner() { return owner; }
};

} // namespace stone

#endif