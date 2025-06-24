#ifndef STONE_CORE_CHARINFO_H
#define STONE_CORE_CHARINFO_H

#include <cstdint>

namespace stone {
namespace charinfo {

/// \brief Classification of Unicode code points used in lexical analysis.
///
/// Used for categorizing characters into meaningful groups such as whitespace,
/// identifier parts, operators, digits, and newlines.
enum class CharKind : uint8_t {
  Unknown,            ///< Code point does not match any known category.
  Whitespace,         ///< Horizontal or vertical whitespace characters.
  IdentifierStart,    ///< Valid as the first character of an identifier.
  IdentifierContinue, ///< Valid as a non-initial character of an identifier.
  OperatorStart,      ///< Valid as the first character of an operator.
  OperatorContinue,   ///< Valid as a non-initial character of an operator.
  Digit,              ///< ASCII digits (0-9).
  Newline             ///< Line-breaking characters (e.g., \n or \r).
};

/// \returns true if the code point is a valid start character for an
/// identifier.
///
/// This includes ASCII letters, underscore, and valid Unicode letters,
/// excluding digits and combining marks.
bool IsIdentifierStart(uint32_t c);

/// \returns true if the code point is a valid continuation character for an
/// identifier.
///
/// This includes identifier start characters, digits, and certain marks
/// (e.g., combining marks and connector punctuation).
bool IsIdentifierContinuation(uint32_t c);

/// \returns true if the code point is a valid start character for an operator.
///
/// This includes ASCII operator symbols and a set of extended Unicode
/// symbolic characters.
bool IsOperatorStart(uint32_t c);

/// \returns true if the code point is a valid continuation character for an
/// operator.
///
/// This includes operator start characters plus combining marks and
/// Unicode symbol ranges typically used for operator extensions.
bool IsOperatorContinuation(uint32_t c);

/// \returns the classified kind of the given Unicode code point.
///
/// This utility simplifies character analysis in lexers or token filters.
CharKind GetCharKind(uint32_t c);

} // namespace charinfo

} // namespace stone

#endif // STONE_CORE_CHARINFO_H
