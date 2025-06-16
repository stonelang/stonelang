#ifndef STONE_AST_FILEUNIT_H
#define STONE_AST_FILEUNIT_H

#include "stone/AST/ASTUnit.h"

#include "llvm/ADT/SmallVector.h"

namespace stone {

class ASTUnit;
class FileUnit : public ASTUnit {
public:
  FileUnit(ASTUnit *parent) : ASTUnit(parent) {}

public:
  virtual ASTUnitKind GetUnitKind() const = 0;
  virtual void Flush() = 0;
};

} // namespace stone

#endif