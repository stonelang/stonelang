#ifndef STONE_SUPPORT_LANGOPTIONS_H
#define STONE_SUPPORT_LANGOPTIONS_H

#include "stone/Support/FileType.h"
#include "stone/Support/Options.h"

#include "llvm/ADT/StringMap.h"
#include "llvm/Option/ArgList.h"

#include <memory>
namespace stone {

/// Access or distribution level of a library.
enum class LibraryLevel : uint8_t {
  /// Application Programming Interface that is publicly distributed so
  /// public decls are really public and only @_spi decls are SPI.
  API = 0,

  /// System Programming Interface that has restricted distribution
  /// all decls in the module are considered to be SPI including public ones.
  SPI,

  /// The library has some other undefined distribution.
  Other
};

class LangOptions final {
  std::unique_ptr<llvm::opt::OptTable> optTable;

public:
  /// \The path the executing program
  llvm::StringRef MainExecutablePath;

  /// \The name of the executing program
  llvm::StringRef MainExecutableName;

  /// Default target triple.
  std::string DefaultTargetTriple;

  /// The current triple
  llvm::Triple CurrentTargetTriple;

  /// The file type for the main input files. Assuming all inputs produce the
  /// same kind of output.
  FileType InputFileType = FileType::INVALID;

  /// The file type for main output files. Assuming all inputs produce the
  /// same kind of output.
  FileType OutputFileType = FileType::INVALID;

  /// \brief The second target for a zippered build
  ///
  /// This represents the target and minimum deployment version for the
  /// second ('variant') target when performing a zippered build.
  /// For example, if the target is x86_64-apple-macosx10.14 then
  /// a target-variant of x86_64-apple-ios12.0-macabi will produce
  /// a zippered binary that can be loaded into both macCatalyst and
  /// macOS processes. A value of 'None' means no zippering will be
  /// performed.
  std::optional<llvm::Triple> TargetVariant;

  /// \The working director
  llvm::SmallString<128> WorkingDirectory;

  /// This represents the diatnostics generated
  bool PrintDiagnostics = true;
  ///
  bool ShowTimer = false;
  /// Use malloc for allocation object object --  ast-nodes for example.
  bool UseMalloc = false;

  /// The path to which we should output statistics files.
  std::string StatsOutputDir;

  /// Follow the compile lifecyle
  bool PrintLifecycle = false;

  /// Trace changes to stats to files in StatsOutputDir.
  bool TraceStats = false;

  /// Profile changes to stats to files in StatsOutputDir.
  bool ProfileEvents = false;

  /// Profile changes to stats to files in StatsOutputDir, grouped by source
  /// entity.
  bool ProfileEntities = false;

  /// Enable 'availability' restrictions for App Extensions.
  bool EnableAppExtensionRestrictions = false;

  /// The libray level to use
  LibraryLevel CurrentLibraryLevel = LibraryLevel::API;

  /// The parsed input arg list
  std::unique_ptr<llvm::opt::InputArgList> InputArgs;

public:
  /// The map of aliases and underlying names of imported or referenced modules.
  llvm::StringMap<llvm::StringRef> ModuleAliasMap;

  /// The name of the module that the invocation is building.
  std::string ModuleName;
  bool HasModuleName() const { return !ModuleName.empty(); }

  /// The name of the library to link against when using this module.
  std::string ModuleLinkName;
  bool HasModuleLinkName() const { return !ModuleLinkName.empty(); }

public:
  LangOptions();

  LangOptions(const LangOptions &) = delete;
  void operator=(const LangOptions &) = delete;
  LangOptions(LangOptions &&) = delete;
  void operator=(LangOptions &&) = delete;

public:
  /// The main options table
  const llvm::opt::OptTable &GetOptTable() const { return *optTable; }
  /// The main options table
  llvm::opt::OptTable &GetOptTable() { return *optTable; }

  /// Return the input args
  llvm::opt::InputArgList &GetInputArgList() { return *InputArgs; }

  /// Set the main exec path
  void SetMainExecutablePath(llvm::StringRef executablePath) {
    MainExecutablePath = executablePath;
  }

  /// \check that there exist the main executable path
  bool HasMainExecutablePath() const {
    return !MainExecutablePath.empty() && MainExecutablePath.size() > 0;
  }

  /// Set the main exec path
  void SetMainExecutableName(llvm::StringRef executableName) {
    MainExecutableName = executableName;
  }

  /// \check that there exist the main executable path
  bool HasMainExecutableName() const {
    return !MainExecutableName.empty() && MainExecutableName.size() > 0;
  }

  /// \return true if there exist a working directory
  bool HasWorkingDirectory() const {
    return !WorkingDirectory.empty() && WorkingDirectory.size() > 0;
  }
  /// Set the working directory
  void SetWorkingDirectory(llvm::StringRef directory) {
    WorkingDirectory = directory;
  }

  /// Set the default target triple
  void SetDefaultTargetTriple(std::string targetTriple) {
    DefaultTargetTriple = targetTriple;
  }

  // Set the current triple
  void SetCurrentTargetTriple(llvm::Triple triple) {
    CurrentTargetTriple = triple;
  }

  /// \ Return the current triple
  bool HasCurrentTargetTriple() const {
    return LangOptions::HasTargetTriple(CurrentTargetTriple);
  }
  /// Print out the options
  void PrintHelp(bool showHidden);

public:
  static bool HasTargetTriple(llvm::Triple triple);
};

} // namespace stone

#endif
