#include "stone/AST/ASTSession.h"
#include "stone/AST/Type.h"
#include "stone/AST/TypeState.h"

using namespace stone;

ASTSession::ASTSession() : identifierTable(allocator) {

#define BUILTIN_TYPE(ID, Parent)                                               \
  Builtin##ID##Type = new (*this) ID##Type(new (*this) BuiltinTypeState(*this));
#include "stone/AST/TypeNode.def"

  // Initialize all of the known identifiers.
#define BUILTIN_IDENTIFIER_WITH_NAME(Name, IdStr)                              \
  Builtin##Name##Identifier = GetIdentifier(IdStr);
#include "stone/AST/BuiltinIdentifiers.def"
}

ASTSession::~ASTSession() {}

Identifier ASTSession::GetIdentifier(llvm::StringRef text) const {
  if (text.empty() || text.data() == nullptr || text.size() == 0) {
    return Identifier();
  }
  auto it = identifierTable.insert({text, 0}).first;
  return Identifier(it->getKeyData());
}

void *stone::AllocateInASTSession(size_t bytes, const ASTSession &session,
                                  unsigned alignment) {
  return session.AllocateMemory(bytes, alignment);
}
