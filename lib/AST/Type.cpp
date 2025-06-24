#include "stone/AST/Type.h"
#include "stone/AST/DeclFlight.h"
#include "stone/AST/Node.h"
#include "stone/AST/TypeFlight.h"

using namespace stone;

Type::Type(TypeFlightBase *flight) : Node(nullptr), flight(flight) {
  assert(flight && "Expected TypeFlight for a Type!");
  // flight->SetOwner(this);
}

bool Type::IsCanType() const {
  // return (flight->HasCanType() && flight->GetCanType() == this);

  // if (flight->GetKind() == TypeKind::Int32) {
  //   auto *int32Flight = static_cast<TypeFlight<TypeKind::Int32> *>(flight);
  // }
  return false;
}
TypeKind Type::GetKind() const {
  // return flight->GetTypeKind();
  return TypeKind::None;
}

bool FunType::classof(const Type *T) {}
