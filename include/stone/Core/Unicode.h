#ifndef STONE_CORE_UNICODE_H
#define STONE_CORE_UNICODE_H

#include "llvm/ADT/SmallVector.h"

namespace stone::unicode {

/// \brief Encodes the given Unicode code point into UTF-8.
/// \returns true if the code point is invalid or not encodable.
bool EncodeToUTF8(unsigned CharValue, llvm::SmallVectorImpl<char> &Result);

/// \brief Returns the number of leading 1 bits in the byte.
/// Used to determine the UTF-8 sequence length.
unsigned CountLeadingOnes8(unsigned char byte);

/// \brief Checks if the byte is a valid start of a UTF-8 code unit.
/// Returns true if it is **not** a continuation byte (i.e. not 0b10xxxxxx).
bool IsStartOfUTF8Character(unsigned char byte);

/// \brief Validates and decodes a UTF-8 code point.
/// Advances `ptr` and returns the decoded code point or `~0U` if invalid.
uint32_t ValidateUTF8CharacterAndAdvance(const char *&ptr, const char *end);

/// \brief Returns true if the given pointer has a UTF-8 BOM sequence (EF BB
/// BF).
bool HasUTF8BOM(const char *ptr);

} // namespace stone::unicode

#endif // STONE_CORE_UNICODE_H
