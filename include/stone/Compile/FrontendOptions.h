#ifndef STONE_COMPILE_FRONTEND_OPTIONS_H
#define STONE_COMPILE_FRONTEND_OPTIONS_H

#include "stone/Support/LangOptions.h"

namespace stone {

enum class FrontendMode : uint8_t {
  None = 0,
#define FRONTEND_MODE(A) A,
#include "stone/Support/Mode.def"
};
class FrontendOptions final {
  friend class Frontend;

public:
  /// The frontend mode that we are in -- parse, type-check, etc.
  FrontendMode Mode = FrontendMode::None;

  /// Indicates that the input(s) should be parsed as the Stone stdlib.
  bool MustParseAsStdLib = false;

  /// Indicates that we must process duplicate files
  bool MustProcessDuplicateInputFile = false;

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

  void ForAllOutputPaths(const CompilerInputFile &input,
                         llvm::function_ref<void(StringRef)> fn) const;

  bool IsOutputFileDirectory() const;
};

} // namespace stone

#endif // STONE_FRONTEND_FRONTENDINPUTFILE_H
