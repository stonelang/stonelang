#include "stone/Compile/FrontendOptions.h"

using namespace stone;

/// If \p fn returns true, exits early and returns true.
bool FrontendOptions::ForEachInput(
    std::function<bool(const InputFile &)> callback) const {

  for (const InputFile &input : inputs) {
    if (callback(input)) {
      return true;
    }
  }
  return false;
}
