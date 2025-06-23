#ifndef STONE_CORE_LEXING_H
#define STONE_CORE_LEXING_H

namespace stone {

class SrcLoc;
class SrcMgr;
class Token;

class Lexing {
public:
  Lexing() {}
  virtual ~Lexing() {}

public:
  virtual Token GetTokenAtLoc(const SrcMgr &sm, SrcLoc loc) = 0;
  virtual SrcLoc GetLocForEndOfToken(const SrcMgr &sm, SrcLoc loc) = 0;
};
} // namespace stone
#endif
