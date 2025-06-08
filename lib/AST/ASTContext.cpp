#include "stone/AST/ASTContext.h"
#include "stone/AST/AST.h"
#include "stone/AST/Type.h"

using namespace stone;

ASTContext::ASTContext() : identifierTable(allocator) {

  // Initialize all of the known identifiers.
  // This is done here because the allocation is not yet initialized.
#define BUILTIN_IDENTIFIER_WITH_NAME(Name, IdStr)                              \
  Builtin##Name##Identifier = GetIdentifier(IdStr);
#include "stone/AST/BuiltinIdentifiers.def"



}

ASTContext::~ASTContext() {}

void *stone::AllocateInASTContext(size_t bytes, const ASTContext &astContext,
                                  unsigned alignment) {
  return astContext.AllocateMemory(bytes, alignment);
}

Identifier ASTContext::GetIdentifier(llvm::StringRef text) const {
  if (text.empty() || text.data() == nullptr || text.size() == 0) {
    return Identifier(nullptr);
  }

  auto it = identifierTable.insert({text, 0}).first;
  return Identifier(it->getKeyData());
}
