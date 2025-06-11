#ifndef STONE_SUPPORT_STATUS_H
#define STONE_SUPPORT_STATUS_H

#include <memory>

namespace stone {

class alignas(1) Status final {
  uint8_t isError : 1;
  uint8_t isDone : 1;

public:
  constexpr Status() : isError(0), isDone(0) {}
  constexpr Status(bool isError, bool isDone = false)
      : isError(isError), isDone(isDone) {}

public:
  [[nodiscard]] bool IsError() const { return isError; }
  [[nodiscard]] bool IsDone() const { return isDone; }
  [[nodiscard]] bool IsSuccess() const { return !IsError(); }
  [[nodiscard]] bool IsErrorOrIsDone() const { return IsError() || IsDone(); }
  [[nodiscard]] int GetFlag() const { return IsError() ? 1 : 0; }

  void SetIsError() { isError = true; }
  void ClearIsError() { isError = false; }
  void SetIsDone() { isDone = true; }
  void ClearIsDone() { isDone = false; }
  void SetIsDoneAndIsError() {
    SetIsError();
    SetIsDone();
  }

  Status &operator|=(Status RHS) {
    isError |= RHS.isError;
    isDone |= RHS.isDone;
    return *this;
  }

  friend Status operator|(Status LHS, Status RHS) { return LHS |= RHS; }

public:
  [[nodiscard]] static constexpr Status Success() { return Status(); }
  [[nodiscard]] static constexpr Status Error() { return Status(true); }
  [[nodiscard]] static constexpr Status Done() { return Status(false, true); }
  [[nodiscard]] static constexpr Status DoneWithError() {
    return Status(true, true);
  }
};

} // namespace stone
#endif
