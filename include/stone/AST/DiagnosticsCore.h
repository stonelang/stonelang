#ifndef STONE_AST_DIAGNOSTICS_CORE_H
#define STONE_AST_DIAGNOSTICS_CORE_H

#include "stone/AST/Diagnostics.h"
#include "stone/Core/LLVM.h"

namespace stone {

template <typename... ArgTypes> struct Diag;
namespace detail {
// These templates are used to help extract the type arguments of the
// DIAG/ERROR/WARNING/NOTE/REMARK/FIXIT macros.
template <typename T> struct DiagWithArguments;

template <typename... ArgTypes> struct DiagWithArguments<void(ArgTypes...)> {
  typedef Diag<ArgTypes...> type;
};

template <typename T> struct StructuredFixItWithArguments;

template <typename... ArgTypes>
struct StructuredFixItWithArguments<void(ArgTypes...)> {
  typedef StructuredFixIt<ArgTypes...> type;
};
} // end namespace detail

enum class StaticSpellingKind : uint8_t;

namespace diag {
// Declare common diagnostics objects with their appropriate types.
#define DIAG(KIND, ID, Options, Text, Signature)                               \
  extern detail::DiagWithArguments<void Signature>::type ID;
#define FIXIT(ID, Text, Signature)                                             \
  extern detail::StructuredFixItWithArguments<void Signature>::type ID;
#include "stone/Core/DiagnosticsCore.def"
} // namespace diag

} // end namespace stone

#endif
