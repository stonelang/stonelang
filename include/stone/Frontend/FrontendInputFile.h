#ifndef STONE_FRONTEND_FRONTENDINPUTFILE_H
#define STONE_FRONTEND_FRONTENDINPUTFILE_H

#include "stone/Core/FileType.h"

#include "llvm/ADT/PointerIntPair.h"
#include "llvm/Support/MemoryBuffer.h"
#include "llvm/Support/Path.h"
#include <string>

namespace stone {
/// An \c FrontendInputFile encapsulates information about an input passed to
/// the invocation.
///
/// FrontendInstance inputs are usually passed on the command line without a
/// leading flag. However, there are clients that use the \c
/// CompilerConfiguration as a library like LLDB and SourceKit that generate
/// their own \c FrontendInputFile instances programmatically. Note that an \c
/// FrontendInputFile need not actually be backed by a physical file, nor does
/// its file name actually reflect its contents. \c FrontendInputFile has a
/// constructor that will try to figure out the file type from the file name if
/// none is provided, but many clients that construct \c FrontendInputFile
/// instances themselves may provide bogus file names with pre-computed kinds.
/// It is imperative that \c FrontendInputFile::GetType be used as a source of
/// truth for this information.
///
/// \warning \c FrontendInputFile takes an unfortunately lax view of the
/// ownership of its primary data. It currently only owns the file name and a
/// copy of any assigned \c PrimaryFileSpecificPaths outright. It is the
/// responsibility of the caller to ensure that an associated memory buffer
/// outlives the \c FrontendInputFile.
class FrontendInputFile final {
public:
};

} // namespace stone

#endif // STONE_FRONTEND_FRONTENDINPUTFILE_H
