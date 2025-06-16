#ifndef STONE_AST_PARAM_LIST_H
#define STONE_AST_PARAM_LIST_H

#include "llvm/ADT/SmallVector.h"

namespace stone {

enum class ParamListKind : uint8_t {
  Any = 0,
  Using,
  Space,
};

class ParamList {
  ParamListKind kind;

public:
  ParamList(ParamListKind kind) : kind(kind) {}
};

class AnyParamList : public ParamList {
public:
  AnyParamList() : ParamList(ParamList::Any) {}
};
class UsingParamList : public ParamList {
public:
  AnyParamList() : ParamList(ParamList::Using) {}
};

class SpaceParamList : public ParamList {
public:
  SpaceParamList() : ParamList(ParamList::Space) {}
};

} // namespace stone

#endif