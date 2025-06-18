#include "stone/AST/ModuleContext.h"
#include "stone/AST/Type.h"
#include "stone/AST/TypeState.h"

using namespace stone;

ModuleContext::ModuleContext(MemoryManager &memory,
                             IdentifierTable &identifiers)
    : memory(memory), identifiers(identifiers) {

#define BUILTIN_TYPE(ID, Parent)                                               \
  Builtin##ID##Type = new (memory) ID##Type(new (memory) BuiltinTypeState());
#include "stone/AST/TypeNode.def"

  // Initialize all of the known identifiers.
#define BUILTIN_IDENTIFIER_WITH_NAME(Name, IdStr)                              \
  Builtin##Name##Identifier = GetIdentifier(IdStr);
#include "stone/AST/BuiltinIdentifiers.def"
}

Identifier ModuleContext::GetIdentifier(llvm::StringRef name) const {
  if (name.empty()) {
    return Identifier();
  }
  auto it = identifierTable.insert({name, 0}).first;
  return Identifier(it->getKeyData());
}