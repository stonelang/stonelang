#ifndef STONE_AST_ARTIFACT_H
#define STONE_AST_ARTIFACT_H

#include "stone/AST/Identifier.h"
#include "stone/AST/MemoryAllocation.h"

#include <stdint.h>

namespace stone {

enum class ArtifactKind : uint8_t {
  None = 0,
  Decl,
  Expr,
  Stmt,
  Type,
  Module,
  ModuleFile,
  Scope,
  // Builtin,
  DeclState,
  TypeState,
  TypeInfluencer,
  DeclInfluencer
};

class Artifact : public MemoryAllocation<Artifact> {
public:
  Artifact();

public:
  virtual ArtifactKind GetArtifactKind() const = 0;
};

} // namespace stone

#endif
