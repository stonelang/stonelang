#ifndef STONE_AST_PARAM_LIST_H
#define STONE_AST_PARAM_LIST_H

#include "stone/AST/Allocation.h"
#include "llvm/ADT/SmallVector.h"

#include <cstdint>
namespace stone {

enum class ParamListKind : uint8_t { None = 0, Space, Using, Some, Just };

class ParamList : public Allocation<ParamList> {
  ParamListKind kind;

public:
  ParamList(ParamListKind kind) : kind(kind) {}
};

class SpaceParamList : public ParamList {
public:
  SpaceParamList() : ParamList(ParamListKind::Space) {}
};

class UsingParamList : public ParamList {
public:
  UsingParamList() : ParamList(ParamListKind::Using) {}
};

class SomeParamList : public ParamList {
public:
  SomeParamList() : ParamList(ParamListKind::Some) {}
};

class JustParamList : public ParamList {
public:
  JustParamList() : ParamList(ParamListKind::Just) {}
};

} // namespace stone

#endif