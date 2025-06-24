#include "stone/Core/CharInfo.h"

#include "clang/Basic/CharInfo.h"

#include <cstring>

using namespace stone;

/// Operator character checks — based on Swift/Clang precedence and UAX #31.
/// Covers a wide range of symbolic and punctuation Unicode ranges.
static bool IsUnicodeOperatorStart(uint32_t c) {
  return (c >= 0x00A1 && c <= 0x00A7) || c == 0x00A9 || c == 0x00AB ||
         c == 0x00AC || c == 0x00AE || c == 0x00B0 || c == 0x00B1 ||
         c == 0x00B6 || c == 0x00BB || c == 0x00BF || c == 0x00D7 ||
         c == 0x00F7 || c == 0x2016 || c == 0x2017 ||
         (c >= 0x2020 && c <= 0x2027) || (c >= 0x2030 && c <= 0x203E) ||
         (c >= 0x2041 && c <= 0x2053) || (c >= 0x2055 && c <= 0x205E) ||
         (c >= 0x2190 && c <= 0x23FF) || (c >= 0x2500 && c <= 0x2775) ||
         (c >= 0x2794 && c <= 0x2BFF) || (c >= 0x2E00 && c <= 0x2E7F) ||
         (c >= 0x3001 && c <= 0x3003) || (c >= 0x3008 && c <= 0x3030);
}

/// Extends operator continuation set to include combining marks and additional
/// stylistic ranges such as emoji modifiers or variation selectors.
static bool IsUnicodeOperatorContinuation(uint32_t c) {
  return IsUnicodeOperatorStart(c) || (c >= 0x0300 && c <= 0x036F) ||
         (c >= 0x1DC0 && c <= 0x1DFF) || (c >= 0x20D0 && c <= 0x20FF) ||
         (c >= 0xFE00 && c <= 0xFE0F) || (c >= 0xFE20 && c <= 0xFE2F) ||
         (c >= 0xE0100 && c <= 0xE01EF);
}

bool charinfo::IsOperatorStart(uint32_t c) {
  static const char ASCIIOpChars[] = "/=-+*%<>!&|^~.?";
  if (c < 0x80) {
    return std::memchr(ASCIIOpChars, c, sizeof(ASCIIOpChars) - 1) != nullptr;
  }
  return IsUnicodeOperatorStart(c);
}

bool charinfo::IsOperatorContinuation(uint32_t c) {
  return charinfo::IsOperatorStart(c) || IsUnicodeOperatorContinuation(c);
}

bool charinfo::IsIdentifierStart(uint32_t c) {
  if (c < 0x80) {
    return (c == '_' || (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'));
  }

  // Must first pass continuation test, then apply exclusions
  if (!charinfo::IsIdentifierContinuation(c))
    return false;

  // Exclude combining marks typically not valid in initial position
  if ((c >= 0x0300 && c <= 0x036F) || (c >= 0x1DC0 && c <= 0x1DFF) ||
      (c >= 0x20D0 && c <= 0x20FF) || (c >= 0xFE20 && c <= 0xFE2F)) {
    return false;
  }

  return true;
}

bool charinfo::IsIdentifierContinuation(uint32_t c) {
  if (c < 0x80) {
    return (c == '_' || (c >= '0' && c <= '9') || (c >= 'A' && c <= 'Z') ||
            (c >= 'a' && c <= 'z'));
  }

  // Unicode Annex X.1: Recommended identifier extensions for continuation
  return c == 0x00A8 || c == 0x00AA || c == 0x00AD || c == 0x00AF ||
         (c >= 0x00B2 && c <= 0x00B5) || (c >= 0x00B7 && c <= 0x00BA) ||
         (c >= 0x00BC && c <= 0x00BE) || (c >= 0x00C0 && c <= 0x00D6) ||
         (c >= 0x00D8 && c <= 0x00F6) || (c >= 0x00F8 && c <= 0x00FF) ||

         (c >= 0x0100 && c <= 0x167F) || (c >= 0x1681 && c <= 0x180D) ||
         (c >= 0x180F && c <= 0x1FFF) ||

         (c >= 0x200B && c <= 0x200D) || (c >= 0x202A && c <= 0x202E) ||
         (c >= 0x203F && c <= 0x2040) || c == 0x2054 ||
         (c >= 0x2060 && c <= 0x206F) ||

         (c >= 0x2070 && c <= 0x218F) || (c >= 0x2460 && c <= 0x24FF) ||
         (c >= 0x2776 && c <= 0x2793) || (c >= 0x2C00 && c <= 0x2DFF) ||
         (c >= 0x2E80 && c <= 0x2FFF) ||

         (c >= 0x3004 && c <= 0x3007) || (c >= 0x3021 && c <= 0x302F) ||
         (c >= 0x3031 && c <= 0x303F) ||

         (c >= 0x3040 && c <= 0xD7FF) ||

         (c >= 0xF900 && c <= 0xFD3D) || (c >= 0xFD40 && c <= 0xFDCF) ||
         (c >= 0xFDF0 && c <= 0xFE44) || (c >= 0xFE47 && c <= 0xFFF8) ||

         (c >= 0x10000 && c <= 0x1FFFD) || (c >= 0x20000 && c <= 0x2FFFD) ||
         (c >= 0x30000 && c <= 0x3FFFD) || (c >= 0x40000 && c <= 0x4FFFD) ||
         (c >= 0x50000 && c <= 0x5FFFD) || (c >= 0x60000 && c <= 0x6FFFD) ||
         (c >= 0x70000 && c <= 0x7FFFD) || (c >= 0x80000 && c <= 0x8FFFD) ||
         (c >= 0x90000 && c <= 0x9FFFD) || (c >= 0xA0000 && c <= 0xAFFFD) ||
         (c >= 0xB0000 && c <= 0xBFFFD) || (c >= 0xC0000 && c <= 0xCFFFD) ||
         (c >= 0xD0000 && c <= 0xDFFFD) || (c >= 0xE0000 && c <= 0xEFFFD);
}

charinfo::CharKind charinfo::GetCharKind(uint32_t c) {
  if (clang::isWhitespace(c))
    return CharKind::Whitespace;
  if (c == '\n' || c == '\r')
    return CharKind::Newline;
  if (clang::isDigit(c))
    return CharKind::Digit;
  if (IsIdentifierStart(c))
    return CharKind::IdentifierStart;
  if (IsIdentifierContinuation(c))
    return CharKind::IdentifierContinue;
  if (IsOperatorStart(c))
    return CharKind::OperatorStart;
  if (IsOperatorContinuation(c))
    return CharKind::OperatorContinue;
  return CharKind::Unknown;
}
