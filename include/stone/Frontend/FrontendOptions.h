#ifndef STONE_FRONTEND_FRONTEND_OPTIONS_H
#define STONE_FRONTEND_FRONTEND_OPTIONS_H

#include "stone/Core/LangOptions.h"
#include "stone/Core/OutputPaths.h"
#include "stone/Core/Src.h"
#include <vector>

namespace stone {

class FrontendSrc final : public Src {
  friend class Frontend;
  friend class FrontendOptions;

  FileSpecificPaths fileSpecificPaths;

public:
  FrontendSrc(llvm::StringRef file, llvm::MemoryBuffer *memoryBuffer = nullptr)
      : Src(file, memoryBuffer) {}

  FrontendSrc(llvm::StringRef file, stone::FileType fileType,
              llvm::MemoryBuffer *memoryBuffer = nullptr)
      : Src(file, fileType, memoryBuffer) {}

public:
  /// Get the output paths for this file.
  FileSpecificPaths GetFileSpecificPaths() const { return fileSpecificPaths; }
};

enum class FrontendAction : uint8_t {
  None = 0,
#define FRONTEND_ACTION(A) A,
#include "stone/Core/Action.def"
};

class FrontendOptions final {
  friend class Frontend;

public:
  /// The frontend action that we are in -- parse, type-check, etc.
  FrontendAction CurrentAction = FrontendAction::None;

  /// The base set of options
  LangOptions CurrentLangOpts;

  std::vector<FrontendSrc> Inputs;

  /// Indicates that the input(s) should be parsed as the Stone stdlib.
  bool MustParseAsStdLib = false;

  /// Indicates that we must process duplicate files
  bool MustProcessDuplicateSrc = false;

  /// In Single-threaded WMO action, all inputs are used
  /// both for importing and compiling.
  bool IsSingleThreadedWMO = false;

  /// Punt where needed to enable batch action experiments.
  bool AreBatchActionChecksBypassed = false;

  /// Recover missing inputs. Note that recovery itself is users responsibility.
  bool ShouldRecoverMissingInputs = false;

public:
  /// Add an input
  void AddInput(llvm::StringRef file, llvm::MemoryBuffer *buffer = nullptr);

  /// Add an input.
  void AddInput(const FrontendSrc input);

  /// Clear all input files
  void ClearInputs();

  /// If \p fn returns true, exits early and returns true.
  bool ForEachInput(std::function<bool(const FrontendSrc &)> callback) const;

  llvm::ArrayRef<FrontendSrc> GetInputs() const { return inputs; }
  unsigned NumOfInputs() const { return inputs.size(); }
  bool HasInputs() const { return !inputs.empty(); }
  const FrontendSrc &GetFirstInput() const { return inputs[0]; }
  FrontendSrc &GetFirstInput() { return inputs[0]; }
  const FrontendSrc &GetLastInput() const { return inputs.back(); }

  LangOptions &GetLangOptions() { return langOpts; }
  const LangOptions &GetLangOptions() const { return langOpts; }

public:
  bool IsOutputFilenameStdout() const;
  bool IsOutputFileDirectory() const;
  bool HasNamedOutputFile() const;
  std::vector<std::string> CopyOutputFilenames() const;

public:
  /// \return true if the given action requires a proper module name
  static bool DoesActionNeedProperModuleName(FrontendAction action);
  /// \return true if the given action only parses without doing other
  /// compilation steps.
  static bool ShouldActionOnlyParse(FrontendAction action);
  /// \return true if the given action should generates IR
  static bool DoesActionGenerateIR(FrontendAction action);
  /// \return true if the given action should generate native code
  static bool DoesActionGenerateNativeCode(FrontendAction action);
  /// \return true if the given action requires the standard library to be
  /// loaded before it is run.
  static bool DoesActionRequireStoneStandardLibrary(FrontendAction action);
  /// \return true if the given action requires input files to be provided.
  static bool DoesActionRequireInputs(FrontendAction action);
  /// \return true if the given action produces output
  static bool DoesActionProduceOutput(FrontendAction action);
  /// \return true if the given action requires input files to be provided.
  static bool DoesActionPerformEndOfPipelineActions(FrontendAction action);
  /// \return true if the given action supports caching.
  static bool DoesActionSupportCompilationCaching(FrontendAction action);
  /// \return the FileType for the action
  static stone::FileType GetActionOutputFileType(FrontendAction action);
  /// \return the string name of the action
  static llvm::StringRef GetActionString(FrontendAction action);
  /// \return true if this is any action.
  static bool IsAnyAction(FrontendAction action);

  void ForAllOutputPaths(const FrontendSrc &input,
                         std::function<void(StringRef)> callbac) const;
};

} // namespace stone

#endif // STONE_FRONTEND_FRONTENDINPUTFILE_H
