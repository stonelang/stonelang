#include "stone/Frontend/FrontendOptions.h"
#include "stone/Core/Status.h"
#include "stone/Frontend/Frontend.h"

using namespace stone;

/// If \p fn returns true, exits early and returns true.
bool FrontendOptions::ForEachInput(
    std::function<bool(const FrontendSrc &)> callback) const {
  for (const FrontendSrc &input : inputs) {
    if (callback(input)) {
      return true;
    }
  }
  return false;
}