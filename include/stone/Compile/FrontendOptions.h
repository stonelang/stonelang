#ifndef STONE_COMPILE_FRONTEND_OPTIONS_H
#define STONE_COMPILE_FRONTEND_OPTIONS_H

#include "stone/Support/InputFile.h"
#include "stone/Support/LangOptions.h"
#include "stone/Support/OutputPaths.h"
#include <vector>

namespace stone {

enum class FrontendMode : uint8_t {
  None = 0,
#define FRONTEND_MODE(A) A,
#include "stone/Support/Mode.def"
};

class FrontendInputFile final : public InputFile {
  friend class Frontend;
  friend class FrontendOptions;

  FileSpecificPaths fileSpecificPaths;

public:
  FrontendInputFile(llvm::StringRef file,
                    llvm::MemoryBuffer *memoryBuffer = nullptr)
      : InputFile(file, memoryBuffer) {}

  FrontendInputFile(llvm::StringRef file, stone::FileType fileType,
                    llvm::MemoryBuffer *memoryBuffer = nullptr)
      : InputFile(file, fileType, memoryBuffer) {}

public:
  /// Get the output paths for this file.
  FileSpecificPaths GetFileSpecificPaths() const { return fileSpecificPaths; }
};

class FrontendOptions final {
  friend class Frontend;

  /// The base set of options
  LangOptions langOpts;

  std::vector<FrontendInputFile> inputs;

public:
  /// The frontend mode that we are in -- parse, type-check, etc.
  FrontendMode Mode = FrontendMode::None;

  /// Indicates that the input(s) should be parsed as the Stone stdlib.
  bool MustParseAsStdLib = false;

  /// Indicates that we must process duplicate files
  bool MustProcessDuplicateInputFile = false;

  /// In Single-threaded WMO mode, all inputs are used
  /// both for importing and compiling.
  bool IsSingleThreadedWMO = false;

  /// Punt where needed to enable batch mode experiments.
  bool AreBatchModeChecksBypassed = false;

  /// Recover missing inputs. Note that recovery itself is users responsibility.
  bool ShouldRecoverMissingInputs = false;

public:
  /// Add an input
  void AddInput(llvm::StringRef file, llvm::MemoryBuffer *buffer = nullptr);

  /// Add an input.
  void AddInput(const FrontendInputFile input);

  /// Clear all input files
  void ClearInputs();

  /// If \p fn returns true, exits early and returns true.
  bool
  ForEachInput(std::function<bool(const FrontendInputFile &)> callback) const;

  llvm::ArrayRef<FrontendInputFile> GetInputs() const { return inputs; }
  unsigned NumOfInputs() const { return inputs.size(); }
  bool HasInputs() const { return !inputs.empty(); }
  const FrontendInputFile &GetFirstInput() const { return inputs[0]; }
  FrontendInputFile &GetFirstInput() { return inputs[0]; }
  const FrontendInputFile &GetLastInput() const { return inputs.back(); }

  LangOptions &GetLangOptions() { return langOpts; }
  const LangOptions &GetLangOptions() const { return langOpts; }

public:
  bool IsOutputFilenameStdout() const;
  bool IsOutputFileDirectory() const;
  bool HasNamedOutputFile() const;
  std::vector<std::string> CopyOutputFilenames() const;

public:
  /// \return true if the given action requires a proper module name
  static bool DoesModeNeedProperModuleName(FrontendMode mode);
  /// \return true if the given action only parses without doing other
  /// compilation steps.
  static bool ShouldModeOnlyParse(FrontendMode mode);
  /// \return true if the given action should generates IR
  static bool DoesModeGenerateIR(FrontendMode mode);
  /// \return true if the given action should generate native code
  static bool DoesModeGenerateNativeCode(FrontendMode mode);
  /// \return true if the given action requires the standard library to be
  /// loaded before it is run.
  static bool DoesModeRequireStoneStandardLibrary(FrontendMode mode);
  /// \return true if the given action requires input files to be provided.
  static bool DoesModeRequireInputs(FrontendMode mode);
  /// \return true if the given action produces output
  static bool DoesModeProduceOutput(FrontendMode mode);
  /// \return true if the given action requires input files to be provided.
  static bool DoesModePerformEndOfPipelineModes(FrontendMode mode);
  /// \return true if the given action supports caching.
  static bool DoesModeSupportCompilationCaching(FrontendMode mode);
  /// \return the FileType for the action
  static stone::FileType GetModeOutputFileType(FrontendMode mode);
  /// \return the string name of the action
  static llvm::StringRef GetModeString(FrontendMode mode);
  /// \return true if this is any action.
  static bool IsAnyMode(FrontendMode mode);

  void ForAllOutputPaths(const FrontendInputFile &input,
                         std::function<void(StringRef)> callbac) const;
};

} // namespace stone

#endif // STONE_FRONTEND_FRONTENDINPUTFILE_H
