#ifndef STONE_AST_ARTIFACT_H
#define STONE_AST_ARTIFACT_H

#include "stone/AST/Identifier.h"
#include "stone/AST/MemoryAllocation.h"

#include <stdint.h>

namespace stone {

enum class ArtifactKind : uint8_t {
  None = 0,
#define ARTIFACT(K) K,
#include "Artifact.def"
};

class Artifact : public MemoryAllocation<Artifact> {
public:
  Artifact();
  virtual ~Artifact() = default;
public:
  // void Walk(ArtifactWalker &walker);
  // void Walk(ArtifactWalker &&walker) { Walk(walker); }
  // void Visit(ASTVisitor& visitor);

  virtual ArtifactKind GetArtifactKind() const = 0;

  /// \brief Pretty-print the artifact for debugging/logging
  virtual void Print(llvm::raw_ostream &os) const {}

  /// \brief Trace memory allocation or ownership metadata
  virtual void TraceAllocation(llvm::raw_ostream &os) const {}

  // virtual void Evaluate(EvaluatorKind kind);
};

} // namespace stone

#endif
