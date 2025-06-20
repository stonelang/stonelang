#ifndef STONE_AST_SPECIALNAME_H
#define STONE_AST_SPECIALNAME_H

#include "stone/AST/Identifier.h"
#include "llvm/ADT/StringRef.h"

namespace stone {

inline bool IsConstructorOf(Identifier name, Identifier type) {
  return name == type;
}

inline bool IsDestructorOf(Identifier name, Identifier type) {
  auto text = name.GetText();
  return text.startswith("~") && text.drop_front() == type.GetText();
}

} // namespace stone
#endif
