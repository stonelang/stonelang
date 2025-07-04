//===----------------------------------------------------------------------===//
///
/// \file
/// Provides info about editor placeholders, <#such as this#>.
///
//===----------------------------------------------------------------------===//
#ifndef STONE_CORE_EDITORPLACEHOLDER_H
#define STONE_CORE_EDITORPLACEHOLDER_H

#include "stone/Core/LLVM.h"
#include "llvm/ADT/StringRef.h"

namespace stone {

enum class EditorPlaceholderKind {
  Basic,
  Typed,
};

struct EditorPlaceholderData {
  /// Placeholder kind.
  EditorPlaceholderKind Kind;
  /// The part that is displayed in the editor.
  llvm::StringRef Display;
  /// If kind is \c Typed, this is the type string for the placeholder.
  llvm::StringRef Type;
  /// If kind is \c Typed, this is the type string to be considered for
  /// placeholder expansion.
  /// It can be same as \c Type or different if \c Type is a typealias.
  llvm::StringRef TypeForExpansion;
};

/// Deconstructs a placeholder string and returns info about it.
/// \returns None if the \c PlaceholderText is not a valid placeholder string.
std::optional<EditorPlaceholderData>
parseEditorPlaceholder(llvm::StringRef PlaceholderText);

/// Checks if an identifier with the given text is an editor placeholder
bool isEditorPlaceholder(llvm::StringRef IdentifierText);
} // end namespace stone

#endif // SWIFT_BASIC_EDITORPLACEHOLDER_H
