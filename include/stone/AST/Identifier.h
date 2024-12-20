#ifndef STONE_AST_IDENTIFIER_H
#define STONE_AST_IDENTIFIER_H

#include "stone/AST/ASTAllocation.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/ADT/StringMapEntry.h"

namespace stone {
class Identifier final : public ASTAllocation<Identifier> {
  friend class ASTContext;

  // A pointer to the data in the entry table
  llvm::StringMapEntry<Identifier *> *entry = nullptr;

public:
  Identifier(const Identifier &) = delete;
  Identifier &operator=(const Identifier &) = delete;
  Identifier(Identifier &&) = delete;
  Identifier &operator=(Identifier &&) = delete;
  Identifier() {}

public:
  // llvm::StringRef GetName() { }
public:
};
} // namespace stone
#endif