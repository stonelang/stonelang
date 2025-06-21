#include "stone/AST/Type.h"
#include "stone/AST/DeclState.h"
#include "stone/AST/TypeState.h"

using namespace stone;

Type::Type(TypeState *TS) : Node(nullptr), TS(TS) {
  assert(TS && "Expected TypeState for a Type!");
  TS->SetOwner(this);
}

bool Type::IsCanType() const {
  return (TS->HasCanType() && TS->GetCanType() == this);
}
TypeKind Type::GetKind() const { return TS->GetTypeKind(); }
