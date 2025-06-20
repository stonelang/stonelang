#include "stone/AST/Scope.h"

using namespace stone;

Decl *Scope::Find(Identifier id) const {
  auto it = symbols.find(id);
  return (it != symbols.end()) ? it->second : nullptr;
}