#ifndef STONE_CORE_SRCUNIT_H
#define STONE_CORE_SRCUNIT_H

#include "stone/Core/FileType.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/Support/Path.h"

namespace stone {
/// \brief Represents a `.stone` source file unit tracked by the compiler.
///
/// A `SrcUnit` encapsulates the normalized file name and the associated
/// buffer ID from the source manager. It is move-only and non-owning.
///
/// - If `bufferID == 0`, the unit is considered **invalid**, typically due to
///   file loading failure or deferred initialization.
/// - The input name `<stdin>` is normalized to `"-"` internally.
/// - Only files with `.stone` extension or `"-"` are accepted when valid.
class SrcUnit final {
  unsigned bufferID;           ///< Source manager-assigned buffer ID (0 = invalid).
  llvm::StringRef name;        ///< Normalized file name (e.g., "main.stone" or "-").
public:
  /// \brief Constructs a `SrcUnit`. If `bufferID == 0`, the unit is invalid.
  ///
  /// \param inputName The file name or `<stdin>`.
  /// \param bufferID The buffer ID assigned by the source manager (default = 0).
  explicit SrcUnit(llvm::StringRef inputName, unsigned bufferID = 0)
      : bufferID(bufferID),
        name(inputName.equals("<stdin>") ? "-" : inputName) {
    assert(!name.empty() && "File name must not be empty!");

    if (IsValid()) {
      using llvm::sys::path::extension;
      assert(name == "-" || extension(name) == ".stone" &&
             "SrcUnit must represent a `.stone` file!");
    }
  }
   /// \returns true if this unit is valid (i.e., has a non-zero buffer ID).
  explicit operator bool() const { return IsValid(); }

  /// \returns true if this unit is valid (i.e., has a non-zero buffer ID).
  bool IsValid() const { return bufferID != 0; }

  /// \returns true if this unit represents standard input.
  bool IsStdin() const { return name == "-"; }

  /// \returns the underlying buffer ID assigned by the source manager.
  unsigned GetBufferID() const { return bufferID; }

  /// \returns the normalized file name (non-owning reference).
  const llvm::StringRef &GetName() const { return name; }

  /// \returns a user-facing name for diagnostics ("<stdin>" or the actual file name).
  llvm::StringRef GetDisplayName() const {
    return IsStdin() ? "<stdin>" : name;
  }

  /// \brief Compile-time tag identifying this unit's file kind.
  static constexpr FileKind Kind = FileKind::Stone;
};
}

#endif // STONE_BASIC_SRC_H
