#ifndef STONE_AST_IDENTIFIER_H
#define STONE_AST_IDENTIFIER_H

#include "llvm/ADT/StringRef.h"

#include <stdint.h>

namespace stone {
class Identifier {
  friend class ASTContext;

  const char *dataPointer = nullptr;
  explicit Identifier(const char *dataPtr = nullptr) : dataPointer(dataPtr) {}

public:
  explicit operator bool() const { return !IsEmpty(); }

public:
  const char *GetPointer() const { return dataPointer; }
  llvm::StringRef GetString() const { return dataPointer; }
  explicit operator std::string() const { return std::string(dataPointer); }

  unsigned GetLength() const {
    assert(dataPointer != nullptr &&
           "Tried getting length of empty identifier");
    return ::strlen(dataPointer);
  }
  bool IsEmpty() const { return dataPointer == nullptr; }
  bool IsEqual(llvm::StringRef other) const {
    return GetString().equals(other);
  }

public:
  bool IsArithmeticOperator() const {
    return IsEqual("+") || IsEqual("-") || IsEqual("*") || IsEqual("/") ||
           IsEqual("%");
  }
  // Returns whether this is a standard comparison operator,
  // such as '==', '>=' or '!=='.
  bool IsSTDComparisonOperator() const {
    return IsEqual("==") || IsEqual("!=") || IsEqual("<") || IsEqual(">") ||
           IsEqual("<=") || IsEqual(">=");
  }

public:
};

} // namespace stone

#endif