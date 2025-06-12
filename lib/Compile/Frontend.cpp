#include "stone/Compile/Frontend.h"

using namespace stone;

// class Frontend::Implementation final {
//   Frontend &frontend;

// public:
//   Implementation(Frontend &frontend) : frontend(frontend) {}

// public:
//   Status ConvertOptions(MemoryBuffers *buffers);

//   //   Status
//   //   ConvertOutputs(std::vector<std::string> &mainOutputs,
//   //                  std::vector<std::string> &mainOutputsForIndexUnits,
//   //                  std::vector<SupplementaryOutputPaths>
//   //                  &supplementaryOutputs, FrontendMode mode);
//   // }
// };

// Status Frontend::Implementation::ConvertOptions(MemoryBuffers *buffers) {}

// // Status Frontend::Implementation::ConvertOutputs(
// //     std::vector<std::string> &mainOutputs,
// //     std::vector<std::string> &mainOutputsForIndexUnits,
// //     std::vector<SupplementaryOutputPaths> &supplementaryOutputs,
// //     FrontendMode mode) {}

// class FrontendImpl final {
// public:
//   Frontend &frontend;

//   llvm::SetVector<llvm::StringRef> Files;
//   /// A place to keep alive any buffers that are loaded as part of setting up
//   /// the invocation inputs.
//   llvm::SmallVector<std::unique_ptr<llvm::MemoryBuffer>, 4>
//       ConfigurationFileBuffers;

// public:
//   FrontendImpl(Frontend &frontend) : frontend(frontend) {}
// };

Frontend::Frontend() {}

Status Frontend::ParseArgStrings(llvm::ArrayRef<const char *> args) {

  // Frontend CL options
  unsigned missingIndex;
  unsigned missingCount;

  return Status::Done();
}
