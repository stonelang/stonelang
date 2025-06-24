#include "stone/Core/Unicode.h"
#include "llvm/Support/MathExtras.h"

using namespace stone;

bool unicode::EncodeToUTF8(unsigned CharValue,
                           llvm::SmallVectorImpl<char> &Result) {
  unsigned NumBits = 32 - llvm::countl_zero(CharValue);
  unsigned NumTrailingBytes = 0;

  if (NumBits <= 5 + 6) {
    Result.push_back(char(0xC0 | (CharValue >> 6)));
    NumTrailingBytes = 1;
  } else if (NumBits <= 4 + 6 + 6) {
    Result.push_back(char(0xE0 | (CharValue >> 12)));
    NumTrailingBytes = 2;
    if ((CharValue >= 0xD800 && CharValue <= 0xDFFF) ||
        (CharValue >= 0xFDD0 && CharValue <= 0xFDEF))
      return true;
  } else if (NumBits <= 3 + 6 + 6 + 6) {
    Result.push_back(char(0xF0 | (CharValue >> 18)));
    NumTrailingBytes = 3;
    if (CharValue > 0x10FFFF)
      return true;
  } else {
    return true;
  }

  while (NumTrailingBytes--)
    Result.push_back(
        char(0x80 | ((CharValue >> (NumTrailingBytes * 6)) & 0x3F)));

  return false;
}

unsigned unicode::CountLeadingOnes8(unsigned char byte) {
  return llvm::countl_one(uint32_t(byte) << 24);
}

bool unicode::IsStartOfUTF8Character(unsigned char byte) {
  return byte <= 0x80 || (byte >= 0xC2 && byte < 0xF5);
}

uint32_t unicode::ValidateUTF8CharacterAndAdvance(const char *&ptr,
                                                  const char *end) {
  if (ptr >= end)
    return ~0U;

  unsigned char first = *ptr++;
  if (first < 0x80)
    return first;

  unsigned numBytes = CountLeadingOnes8(first);
  if (numBytes < 2 || !IsStartOfUTF8Character(first)) {
    while (ptr < end && !IsStartOfUTF8Character(*ptr))
      ++ptr;
    return ~0U;
  }

  unsigned charValue = (unsigned char)(first << numBytes) >> numBytes;
  for (unsigned i = 1; i < numBytes; ++i) {
    if (ptr >= end)
      return ~0U;
    unsigned char cont = *ptr;
    if (cont < 0x80 || cont >= 0xC0)
      return ~0U;
    charValue = (charValue << 6) | (cont & 0x3F);
    ++ptr;
  }

  if (charValue >= 0xD800 && charValue <= 0xDFFF)
    return ~0U;

  unsigned numBits = 32 - llvm::countl_zero(charValue);
  if (numBits <= 5 + 6)
    return numBytes == 2 ? charValue : ~0U;
  if (numBits <= 4 + 6 + 6)
    return numBytes == 3 ? charValue : ~0U;
  return numBytes == 4 ? charValue : ~0U;
}

bool unicode::HasUTF8BOM(const char *ptr) {
  return ptr[0] == '\xEF' && ptr[1] == '\xBB' && ptr[2] == '\xBF';
}
