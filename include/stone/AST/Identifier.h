#ifndef STONE_AST_IDENTIFIER_H
#define STONE_AST_IDENTIFIER_H

#include "stone/Support/EditorPlaceholder.h"
#include "llvm/Support/Allocator.h"

#include "llvm/ADT/StringRef.h"

#include <stdint.h>

namespace stone {
class Identifier {
  friend class DeclName;
  friend class DeclNameBase;
  friend class ASTSession;
  const char *dataPointer = nullptr;
  explicit Identifier(const char *dataPtr) : dataPointer(dataPtr) {}

public:
  Identifier() : Identifier(nullptr) {}
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
  /// isOperatorStartCodePoint - Return true if the specified code point is a
  /// valid start of an operator.
  static bool IsOperatorStartCodePoint(uint32_t C) {
    // ASCII operator chars.
    static const char OpChars[] = "/=-+*%<>!&|^~.?";
    if (C < 0x80) {
      return memchr(OpChars, C, sizeof(OpChars) - 1) != 0;
    }

    // Unicode math, symbol, arrow, dingbat, and line/box drawing chars.
    return (C >= 0x00A1 && C <= 0x00A7) || C == 0x00A9 || C == 0x00AB ||
           C == 0x00AC || C == 0x00AE || C == 0x00B0 || C == 0x00B1 ||
           C == 0x00B6 || C == 0x00BB || C == 0x00BF || C == 0x00D7 ||
           C == 0x00F7 || C == 0x2016 || C == 0x2017 ||
           (C >= 0x2020 && C <= 0x2027) || (C >= 0x2030 && C <= 0x203E) ||
           (C >= 0x2041 && C <= 0x2053) || (C >= 0x2055 && C <= 0x205E) ||
           (C >= 0x2190 && C <= 0x23FF) || (C >= 0x2500 && C <= 0x2775) ||
           (C >= 0x2794 && C <= 0x2BFF) || (C >= 0x2E00 && C <= 0x2E7F) ||
           (C >= 0x3001 && C <= 0x3003) || (C >= 0x3008 && C <= 0x3030);
  }
  /// isOperatorContinuationCodePoint - Return true if the specified code point
  /// is a valid operator code point.
  static bool IsOperatorContinuationCodePoint(uint32_t C) {
    if (IsOperatorStartCodePoint(C)) {
      return true;
    }
    // Unicode combining characters and variation selectors.
    return (C >= 0x0300 && C <= 0x036F) || (C >= 0x1DC0 && C <= 0x1DFF) ||
           (C >= 0x20D0 && C <= 0x20FF) || (C >= 0xFE00 && C <= 0xFE0F) ||
           (C >= 0xFE20 && C <= 0xFE2F) || (C >= 0xE0100 && C <= 0xE01EF);
  }

  static bool IsEditorPlaceholder(llvm::StringRef name) {
    return stone::isEditorPlaceholder(name);
  }

  bool IsEditorPlaceholder() const {
    return !IsEmpty() && isEditorPlaceholder(GetString());
  }

  const void *GetAsOpaquePointer() const {
    return static_cast<const void *>(dataPointer);
  }

  static Identifier GetFromOpaquePointer(void *Ptr) {
    return Identifier((const char *)Ptr);
  }

  /// Compare two identifiers, producing -1 if \c *this comes before \c other,
  /// 1 if \c *this comes after \c other, and 0 if they are equal.
  ///
  /// Null identifiers come after all other identifiers.
  // int Compare(Identifier other) const;

  // friend llvm::hash_code hash_value(Identifier identifier) {
  //   return llvm::hash_value(identifier.GetAsOpaquePointer());
  // }

  bool operator==(Identifier RHS) const {
    return dataPointer == RHS.dataPointer;
  }
  bool operator!=(Identifier RHS) const { return !(*this == RHS); }
  bool operator<(Identifier RHS) const { return dataPointer < RHS.dataPointer; }
};

using IdentifierTable = llvm::StringMap<char, llvm::BumpPtrAllocator &>;
} // namespace stone

#endif