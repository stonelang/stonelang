#ifndef STONE_DIAG_ARTIFACT_KIND_H
#define STONE_DIAG_ARTIFACT_KIND_H

#include "llvm/ADT/StringRef.h"
#include <string>

namespace stone {

/// \brief Represents the kind of data (artifact) attached to a diagnostic.
///
/// Diagnostic artifacts are used to parameterize diagnostics with additional
/// context (e.g., names, types, values). This enum identifies the type of each
/// artifact in a compact, type-safe way.
///
/// This kind system supports diagnostic formatting, serialization, and dynamic
/// rendering of diagnostic messages.
enum class DiagArtifactKind : uint8_t {
  None = 0,     ///< No artifact (placeholder or uninitialized)
  Bool,         ///< Boolean value (true/false)
  String,       ///< String or string-like (std::string, StringRef)
  Integer,      ///< Signed integer (int, int32_t, etc.)
  Unsigned,     ///< Unsigned integer (unsigned, uint32_t, etc.)
  Void,         ///< Marker for "no value" or unused
  Decl,         ///< AST declaration pointer
  Type,         ///< AST type pointer
  Identifier    ///< Compiler-internal identifier (symbol/token)
};

/// \brief Trait that maps C++ types to their corresponding DiagArtifactKind.
///
/// This template is specialized for each supported type to allow compile-time
/// reflection of artifact kind information. It's used when constructing or formatting
/// diagnostics that take arguments of different types.
template <typename T>
struct DiagArtifactKindOf; // Undefined base — will fail if unsupported type is used

// === String types ===
template <>
struct DiagArtifactKindOf<llvm::StringRef> {
  static constexpr DiagArtifactKind kind = DiagArtifactKind::String;
};

template <>
struct DiagArtifactKindOf<std::string> {
  static constexpr DiagArtifactKind kind = DiagArtifactKind::String;
};

// === Integer types ===
template <>
struct DiagArtifactKindOf<int> {
  static constexpr DiagArtifactKind kind = DiagArtifactKind::Integer;
};

template <>
struct DiagArtifactKindOf<unsigned> {
  static constexpr DiagArtifactKind kind = DiagArtifactKind::Unsigned;
};

template <> struct DiagArtifactKindOf<void*> {
  static constexpr DiagArtifactKind kind = DiagArtifactKind::Void; // or Void
};

// === AST-specific types ===
template <>
struct DiagArtifactKindOf<Decl*> {
  static constexpr DiagArtifactKind kind = DiagArtifactKind::Decl;
};

template <>
struct DiagArtifactKindOf<Type*> {
  static constexpr DiagArtifactKind kind = DiagArtifactKind::Type;
};

template <>
struct DiagArtifactKindOf<Identifier> {
  static constexpr DiagArtifactKind kind = DiagArtifactKind::Identifier;
};

} // namespace stone

#endif // S
