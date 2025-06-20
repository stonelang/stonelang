#ifndef STONE_AST_FILEARTIFACT_H
#define STONE_AST_FILEARTIFACT_H

#include "stone/AST/Artifact.h"

#include "llvm/ADT/SmallVector.h"

namespace stone {

class FileArtifact : public Artifact {
public:
  FileArtifact(Artifact *parent) {}

public:
  virtual ArtifactKind GetArtifactKind() const = 0;
  virtual void Flush() = 0;
};

} // namespace stone

#endif