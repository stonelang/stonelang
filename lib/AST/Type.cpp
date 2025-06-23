#include "stone/AST/Type.h"
#include "stone/AST/DeclFlight.h"
#include "stone/AST/Node.h"
#include "stone/AST/TypeFlight.h"

using namespace stone;

Type::Type(TypeFlight *flight) : flight(flight) {
  assert(flight && "Expected TypeFlight for a Type!");
  flight->SetOwner(this);
}

bool Type::IsCanType() const {
  return (flight->HasCanType() && flight->GetCanType() == this);
}
TypeKind Type::GetKind() const { return flight->GetTypeKind(); }

bool FunType::classof(const Type *T) {}
