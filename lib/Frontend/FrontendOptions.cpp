#include "stone/Support/Status.h"
#include "stone/Frontend/Frontend.h"
#include "stone/Frontend/FrontendOptions.h"

using namespace stone;

/// If \p fn returns true, exits early and returns true.
bool FrontendOptions::ForEachInput(
    std::function<bool(const FrontendInputFile &)> callback) const {
  for (const FrontendInputFile &input : inputs) {
    if (callback(input)) {
      return true;
    }
  }
  return false;
}

Status Frontend::ParseFrontendOptions() { return Status::Success(); }