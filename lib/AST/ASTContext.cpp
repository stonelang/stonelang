#include "stone/AST/ASTContext.h"

using namespace stone;

Identifier &ASTContext::GetIdentifier(llvm::StringRef identStr) {
  auto &entry = *identifiers.try_emplace(identStr, nullptr).first;
  Identifier *&ident = entry.second;
  if (ident) {
    return *ident;
  }
  ident = new (*this) Identifier();
  // Set a pointer to the data 
  ident->entry = &entry;
  return *ident;
}
