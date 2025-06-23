#include "stone/AST/DiagID.h"

namespace stone::diag {

#define DIAG(KIND, ID, Options, Text, Signature)                               \
  detail::DiagWithArguments<void Signature>::type ID = {DiagID::ID};

#define FIXIT(ID, Text, Signature)                                             \
  detail::StructuredFixItWithArguments<void Signature>::type ID = {FixItID::ID};

#include "stone/Core/Diagnostics.def"

} // namespace stone::diag
