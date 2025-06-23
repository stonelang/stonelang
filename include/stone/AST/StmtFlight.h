#ifndef STONE_AST_STMTSTATE_H
#define STONE_AST_STMTSTATE_H

#include "stone/AST/Alignments.h"
#include "stone/AST/Allocation.h"
#include "stone/AST/Number.h"
#include "stone/AST/StmtKind.h"
#include "stone/AST/TypeInfluencer.h"
#include "stone/AST/TypeKind.h"

namespace stone {

class alignas(8) StmtFlight : public Allocation<StmtFlight> {
  StmtKind kind;

public:
};

} // namespace stone

#endif