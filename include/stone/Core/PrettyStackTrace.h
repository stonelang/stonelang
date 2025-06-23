#ifndef STONE_CORE_PRETTYSTACKTRACE_H
#define STONE_CORE_PRETTYSTACKTRACE_H

#include "stone/Core/SrcLoc.h"
#include "llvm/Support/PrettyStackTrace.h"

namespace stone {
/// If a crash happens while one of these objects are live, the message
/// is printed out along with the specified source location.
class PrettyStackTraceLoc final : public llvm::PrettyStackTraceEntry {
  SrcMgr &sm;
  SrcLoc loc;
  const char *message;

public:
  PrettyStackTraceLoc(SrcMgr &sm, SrcLoc loc, const char *msg)
      : sm(sm), loc(loc), message(msg) {}
  // TODO:
  void print(raw_ostream &OS) const override;
};
} // namespace stone
#endif
