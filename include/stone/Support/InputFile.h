#ifndef STONE_SUPPORT_INPUTFILE_H
#define STONE_SUPPORT_INPUTFILE_H

#include "stone/Support/FileType.h"
#include "llvm/ADT/SmallVector.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/Support/MemoryBuffer.h"

namespace stone {

class InputFile final {
  llvm::StringRef fileName;
  FileType fileType;

public:
  InputFile(const InputFile &) = delete;
  void operator=(const InputFile &) = delete;

public:
  InputFile(llvm::StringRef fileName)
      : InputFile(fileName, stone::GetFileTypeByExt(
                                llvm::sys::path::extension(fileName))) {}

  InputFile(llvm::StringRef fileName, FileType fileType)
      : fileName(TranslateName(fileName)), fileType(fileType) {
    assert(!fileName.empty() && "File name must not be empty");
  }

public:
  const llvm::StringRef &GetFileName() const { return fileName; }
  FileType GetFileType() const { return fileType; }

public:
  bool IsStone() const { return fileType == FileType::Stone; }
  bool IsSpace() const { return fileType == FileType::Space; }
  bool IsObject() const { return fileType == FileType::Object; }
  bool IsIR() const { return fileType == FileType::IR; }
  bool IsBC() const { return fileType == FileType::BC; }
  bool IsAssembly() const { return fileType == FileType::Assembly; }
  bool IsStaticLibrary() const { return fileType == FileType::StaticLibrary; }

  void ExpectStone() const {
    assert(IsStone() && "Expected a .stone source file");
  }
  void Expect(FileType expected) const {
    assert(fileType == expected && "Unexpected file type");
  }

public:
  static llvm::StringRef TranslateName(llvm::StringRef fileName) {
    return fileName.equals("<stdin>") ? "-" : fileName;
  }
};
using InputFileList = llvm::SmallVector<const InputFile, 16>;

} // namespace stone
#endif