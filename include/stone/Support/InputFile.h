#ifndef STONE_SUPPORT_INPUTFILE_H
#define STONE_SUPPORT_INPUTFILE_H

#include "stone/Support/FileType.h"
#include "llvm/ADT/SmallVector.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/Support/MemoryBuffer.h"

namespace stone {

/// Return stone-standard file name from a buffer name set by
/// llvm::MemoryBuffer::getFileOrSTDIN, which uses "<stdin>" instead of "-".
llvm::StringRef ConvertBufferNameFromLLVMGetFileOrSTDINToStoneConventions(
    llvm::StringRef fileName);

class InputFile final {
  llvm::StringRef fileName;
  FileType fileType;
  llvm::MemoryBuffer *memoryBuffer = nullptr;

public:
  InputFile(llvm::StringRef fileName,
            llvm::MemoryBuffer *memoryBuffer = nullptr)
      : InputFile(fileName,
                  stone::GetFileTypeByExt(llvm::sys::path::extension(fileName)),
                  memoryBuffer) {}

  /// Constructs an input file from the provided data.
  InputFile(llvm::StringRef fileName, stone::FileType fileType,
            llvm::MemoryBuffer *memoryBuffer = nullptr)
      : fileName(
            stone::ConvertBufferNameFromLLVMGetFileOrSTDINToStoneConventions(
                fileName)),
        fileType(fileType), memoryBuffer(memoryBuffer) {
    assert(!fileName.empty());
  }

public:
  /// The returned file name is guaranteed not to be the empty string.
  const llvm::StringRef &GetFileName() const {
    assert(!fileName.empty());
    return fileName;
  }
  FileType GetFileType() const { return fileType; }

  bool HasBuffer() const { return memoryBuffer != nullptr; }
  /// Retrieves the backing buffer for this input file, if any.
  llvm::MemoryBuffer *GetBuffer() const { return memoryBuffer; }

  bool IsStoneFileType() const { return GetFileType() == FileType::Stone; }
  bool IsObjectFileType() const { return GetFileType() == FileType::Object; }
  bool IsIRFileType() const { return GetFileType() == FileType::IR; }
};

using InputFiles = llvm::SmallVector<InputFile, 16>;

} // namespace stone
#endif