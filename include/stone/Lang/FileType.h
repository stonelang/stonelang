#ifndef STONE_LANG_FILE_TYPE_H
#define STONE_LANG_FILE_TYPE_H

namespace stone {
namespace filetype {
enum FileType : unsigned {
  None = 0,
#define FILE_TYPE(TYPE, NAME, EXT) TYPE,
#include "stone/Lang/FileType.def"
#undef FILE_TYPE
  INVALID
};
} // namespace filetype
using FileType = filetype::FileType;

} // namespace stone
#endif