#ifndef STONE_AST_MODULECONTEXT_H
#define STONE_AST_MODULECONTEXT_H

#include "stone/AST/Diagnostics.h"
#include "stone/AST/Identifier.h"
#include "stone/AST/Memory.h"

#include "llvm/ADT/ArrayRef.h"
#include "llvm/ADT/SetVector.h"
#include "llvm/ADT/SmallVector.h"
#include "llvm/ADT/StringMap.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/Support/Allocator.h"
#include "llvm/Support/Casting.h"

namespace stone {

/// \brief ModuleContext manages the core shared state required for building and
/// analyzing a Stone module.
///
/// It owns references to the memory allocator, identifier table, and
/// diagnostics engine, and provides access to built-in types and identifiers.
class ModuleContext final {
  Memory &memory;
  IdentifierTable &identifiers;
  DiagnosticEngine &diags;

public:
  /// Built-in primitive types (e.g., `BuiltinInt32Type`)
  // #define BUILTIN_TYPE(ID, Parent) const Type *Builtin##ID##Type;
  // #include "stone/AST/TypeNode.def"

  /// Built-in reserved identifiers (e.g., `BuiltinIfIdentifier`, etc.)
#define BUILTIN_IDENTIFIER_WITH_NAME(Name, IdStr)                              \
  Identifier Builtin##Name##Identifier;
#include "stone/AST/BuiltinIdentifiers.def"

  /// \returns The canonicalized Identifier for a given string.
  Identifier GetIdentifier(llvm::StringRef text) const;

public:
  /// \brief Constructs a ModuleContext with shared infrastructure.
  ModuleContext(Memory &memory, IdentifierTable &identifiers,
                DiagnosticEngine &diags);

  /// \returns The memory manager used for AST and type allocation.
  Memory &GetMemory() { return memory; }

  /// \returns A canonical Identifier for the given name.
  Identifier GetIdentifier(llvm::StringRef name);

  /// \returns The diagnostics engine for emitting errors, warnings, and notes.
  DiagnosticEngine &GetDiags() { return diags; }
};

} // namespace stone

#endif // STONE_AST_MODULECONTEXT_H
