#ifndef STONE_AST_MODULECONEXT_H
#define STONE_AST_MODULECONEXT_H

#include "stone/AST/Identifier.h"
#include "stone/AST/MemoryManager.h"

#include "llvm/ADT/ArrayRef.h"
#include "llvm/ADT/SetVector.h"
#include "llvm/ADT/SmallVector.h"
#include "llvm/ADT/StringMap.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/Support/Allocator.h"
#include "llvm/Support/Casting.h"

namespace stone {

class ModuleContext final {
  MemoryManager &memory;
  IdentifierTable &identifiers;

public:
  // Declare the set of builtin identifiers.
#define BUILTIN_TYPE(ID, Parent) const Type *Builtin##ID##Type;
#include "stone/AST/TypeNode.def"

  // Declare the set of builtin identifiers.
#define BUILTIN_IDENTIFIER_WITH_NAME(Name, IdStr)                              \
  Identifier Builtin##Name##Identifier;
#include "stone/AST/BuiltinIdentifiers.def"
  Identifier GetIdentifier(llvm::StringRef text) const;

public:
  ModuleContext(MemoryManager &memory, IdentifierTable &identifiers);

public:
  MemoryManager &GetMemory() { return memory; }
  Identifier GetIdentifier(llvm::StringRef name);
};

} // namespace stone

#endif // LANG_MODULE_CONTEXT_H
