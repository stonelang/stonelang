#ifndef STONE_AST_EXPRSTATE_H
#define STONE_AST_TYPESTATE_H

#include "stone/AST/Artifact.h"
#include "stone/AST/MemoryAllocation.h"
#include "stone/AST/Number.h"
#include "stone/AST/TypeAlignment.h"
#include "stone/AST/TypeInfluencer.h"
#include "stone/AST/TypeKind.h"

namespace stone {

class alignas(8) ExprState : public Artifact {};

} // namespace stone

#endif