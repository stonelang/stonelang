#include "stone/AST/Type.h"
#include "stone/AST/DeclState.h"
#include "stone/AST/TypeState.h"

using namespace stone;

Type::Type(TypeState *TS) : Tree(nullptr), TS(TS) {
  assert(TS && "Expected TypeState for a Type!");
  TS->SetOwner(this);
}