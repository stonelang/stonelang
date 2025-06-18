#include "stone/Support/FileType.h"

#include "llvm/ADT/STLExtras.h"
#include "llvm/ADT/StringSwitch.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Support/Path.h"

using namespace stone;
using namespace stone::filetype;

struct FileTypeInfo final {
  const FileType Type;
  const char *Name;
  const char *Ext;
};

static const FileTypeInfo FileTypeInfoList[] = {
#define FILE_TYPE(TYPE, NAME, EXT) {FileType::TYPE, NAME, EXT},
#include "stone/Support/FileType.def"
#undef FILE_TYPE
};

static const FileTypeInfo &GetFileTypeInfo(FileType fileType) {
  assert(stone::IsAnyFileType(fileType) && "Invalid FileType.");
  return FileTypeInfoList[static_cast<unsigned>(fileType)];
}

bool stone::IsAnyFileType(FileType fileType) {
  switch (fileType) {
  case FileType::Stone:
  case FileType::Space:
  case FileType::SpaceDoc:
  case FileType::Spaces:
  case FileType::Object:
  case FileType::IR:
  case FileType::BC:
  case FileType::Assembly:
  case FileType::StaticLibrary:
  case FileType::DynamicLibrary:
  case FileType::Executable:
  case FileType::Diagnostics:
  case FileType::Dependencies:
  case FileType::Autolink:
  case FileType::PCH:
  case FileType::TBD:
    return true;
  default:
    return false;
  }
}

llvm::StringRef stone::GetFileTypeName(FileType fileType) {
  return GetFileTypeInfo(fileType).Name;
}

llvm::StringRef stone::GetFileTypeExt(FileType fileType) {
  return GetFileTypeInfo(fileType).Ext;
}

FileType stone::GetFileTypeByExt(const llvm::StringRef ext) {
  if (ext.empty())
    return FileType::INVALID;
  assert(ext.front() == '.' && "not a file extension");
  return llvm::StringSwitch<FileType>(ext.drop_front())
#define FILE_TYPE(TYPE, NAME, EXT) .Case(EXT, FileType::TYPE)
#include "stone/Support/FileType.def"
      .Default(FileType::INVALID);
}

FileType stone::GetFileTypeByName(const llvm::StringRef name) {
  if (name.empty())
    return FileType::INVALID;
  return llvm::StringSwitch<FileType>(name)
#define FILE_TYPE(TYPE, NAME, EXT) .Case(NAME, FileType::TYPE)
#include "stone/Support/FileType.def"
      .Default(FileType::INVALID);
}

FileType stone::GetFileTypeByPath(const llvm::StringRef path) {
  if (!llvm::sys::path::has_extension(path))
    return FileType::INVALID;
  auto ext = llvm::sys::path::extension(path).str();
  auto fileType = stone::GetFileTypeByExt(ext);
  if (fileType == FileType::INVALID) {
    auto pathStem = llvm::sys::path::stem(path);
    while (llvm::sys::path::has_extension(pathStem)) {
      auto nextExtension = llvm::sys::path::extension(pathStem);
      pathStem = llvm::sys::path::stem(pathStem);
      ext = nextExtension.str() + ext;
      fileType = stone::GetFileTypeByExt(ext);
      if (fileType != FileType::INVALID)
        break;
    }
  }
  return fileType;
}

bool stone::IsTextual(FileType ty) {
  switch (ty) {
  case FileType::Stone:
  case FileType::Assembly:
  case FileType::IR:
  case FileType::Space:
  case FileType::SpaceDoc:
  case FileType::Diagnostics:
  case FileType::Dependencies:
  case FileType::TBD:
    return true;
  default:
    return false;
  }
}

bool stone::IsNoneFileType(FileType fileType) {
  return fileType == FileType::None;
}

bool stone::IsStoneFileType(FileType fileType) {
  return fileType == FileType::Stone;
}

bool stone::IsObjectFileType(FileType fileType) {
  return fileType == FileType::Object;
}

bool stone::IsOutputFileType(FileType fileType) {
  switch (fileType) {
  case FileType::Space:
  case FileType::SpaceDoc:
  case FileType::Spaces:
  case FileType::Object:
  case FileType::IR:
  case FileType::BC:
  case FileType::Assembly:
  case FileType::StaticLibrary:
  case FileType::DynamicLibrary:
  case FileType::Executable:
  case FileType::Diagnostics:
  case FileType::Dependencies:
  case FileType::Autolink:
  case FileType::PCH:
  case FileType::TBD:
    return true;
  default:
    return false;
  }
}

bool stone::IsInputFileType(FileType fileType) {
  switch (fileType) {
  case FileType::Stone:
  case FileType::IR:
  case FileType::Object:
  case FileType::Autolink:
  case FileType::PCH:
    return true;
  default:
    return false;
  }
}

bool stone::IsAfterLLVM(FileType ty) {
  switch (ty) {
  case FileType::Assembly:
  case FileType::IR:
  case FileType::BC:
  case FileType::Object:
  case FileType::StaticLibrary:
  case FileType::DynamicLibrary:
  case FileType::Executable:
    return true;
  default:
    return false;
  }
}

bool stone::IsPartOfStoneCompilation(FileType ty) {
  switch (ty) {
  case FileType::Stone:
  case FileType::Space:
    return true;
  default:
    return false;
  }
}

bool stone::IsPartOfLLVMCompilation(FileType ty) {
  switch (ty) {
  case FileType::IR:
  case FileType::BC:
  case FileType::Object:
    return true;
  default:
    return false;
  }
}

bool stone::ShouldCompileFileType(FileType ty) {
  switch (ty) {
  case FileType::Stone:
    return true;
  default:
    return false;
  }
}

bool stone::CanLinkFileType(FileType ty) {
  switch (ty) {
  case FileType::Object:
  case FileType::StaticLibrary:
  case FileType::DynamicLibrary:
    return true;
  default:
    return false;
  }
}

bool stone::FileExists(llvm::StringRef name) {
  return llvm::sys::fs::exists(name);
}

llvm::StringRef stone::GetFileExt(llvm::StringRef name) {
  return llvm::sys::path::extension(name);
}

llvm::StringRef stone::GetFilePath(llvm::StringRef name) {
  return llvm::sys::path::root_name(name);
}

llvm::StringRef stone::GetFileStem(llvm::StringRef path) {
  return llvm::sys::path::stem(path);
}

bool stone::CanExecuteFile(std::string path) {
  return llvm::sys::fs::can_execute(llvm::Twine(path));
}
