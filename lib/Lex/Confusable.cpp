#include "stone/Lex/Confusable.h"

char stone::ConvertConfusableCharacterToASCII(uint32_t codepoint) {
  switch (codepoint) {
#define CONFUSABLE(CONFUSABLE_POINT, CONFUSABLE_NAME, BASE_POINT, BASE_NAME)   \
  case CONFUSABLE_POINT:                                                       \
    return BASE_POINT;
#include "stone/Lex/Confusable.def"
  default:
    return 0;
  }
}

std::pair<llvm::StringRef, llvm::StringRef>
stone::GetConfusableAndBaseCodepointNames(uint32_t codepoint) {
  switch (codepoint) {
#define CONFUSABLE(CONFUSABLE_POINT, CONFUSABLE_NAME, BASE_POINT, BASE_NAME)   \
  case CONFUSABLE_POINT:                                                       \
    return std::make_pair(CONFUSABLE_NAME, BASE_NAME);
#include "stone/Lex/Confusable.def"
  default:
    return std::make_pair("", "");
  }
}
