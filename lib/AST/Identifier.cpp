#include "stone/AST/Identifier.h"

using namespace stone;

static Identifier
Identifier::FromString(llvm::StringRef identifierText,
                       llvm::StringSet<llvm::BumpPtrAllocator> &table) {
  auto result = table.insert(identifierText);
  return Identifier(result.first->getKeyData());
}

bool Identifier::IsArithmeticOperator() const {
  return IsEqual("+") || IsEqual("-") || IsEqual("*") || IsEqual("/") ||
         IsEqual("%");
}

bool Identifier::IsSTDComparisonOperator() const {
  return IsEqual("==") || IsEqual("!=") || IsEqual("===") || IsEqual("!==") ||
         IsEqual("<") || IsEqual(">") || IsEqual("<=") || IsEqual(">=");
}

bool Identifier::IsOperatorStartCodePoint(uint32_t C) {
  static const char OpChars[] = "/=-+*%<>!&|^~.?";
  if (C < 0x80)
    return memchr(OpChars, C, sizeof(OpChars) - 1) != 0;

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

bool Identifier::IsOperatorContinuationCodePoint(uint32_t C) {
  return IsOperatorStartCodePoint(C) || (C >= 0x0300 && C <= 0x036F) ||
         (C >= 0x1DC0 && C <= 0x1DFF) || (C >= 0x20D0 && C <= 0x20FF) ||
         (C >= 0xFE00 && C <= 0xFE0F) || (C >= 0xFE20 && C <= 0xFE2F) ||
         (C >= 0xE0100 && C <= 0xE01EF);
}
