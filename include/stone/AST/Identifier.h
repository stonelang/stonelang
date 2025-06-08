#ifndef STONE_AST_IDENTIFIER_H
#define STONE_AST_IDENTIFIER_H

namespace stone {
class Identifier {
  friend class ASTContext;

  const char *dataPointer = nullptr;

  explicit Identifier(const char *dataPtr) : dataPointer(dataPtr) {}

public:
  explicit Identifier() : dataPointer(nullptr) {}

  const char *GetPointer() const { return dataPointer; }
};

} // namespace stone

#endif