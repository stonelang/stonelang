#ifndef STONE_AST_DIAGNOSTIC_EMITTER_H
#define STONE_AST_DIAGNOSTIC_EMITTER_H

#include "stone/Basic/SourceLocation.h"
#include "stone/Basic/SrcMgr.h"
#include "stone/Diag/DiagnosticClient.h"

#include "llvm/ADT/PointerUnion.h"

namespace stone {
class SourceLocation;
class LangOptions;
class DiagnosticOptions;

namespace diags {

class DiagnosticInfo;
class StoredDiagnostic;

class PresumedSourceLocation {
  //   unsigned BufferID;
  //   unsigned Line, Col;

  // public:
  //   PresumedSourceLocation() = default;
  //   PresumedSourceLocation(unsigned BufferID, unsigned Line, unsigned Col)
  //       : Filename(FN), BufferID(BufferID), Line(Line), Col(Col) {}

  // public:
  //   bool IsValid() const { return (BufferID > 0); }

  //   unsigned GetBufferID() const {
  //     assert(IsValid());
  //     return BufferID;
  //   }

  //   /// Return the presumed line number of this location.
  //   ///
  //   /// This can be affected by \#line etc.
  //   unsigned GetLine() const {
  //     assert(IsValid());
  //     return Line;
  //   }

  //   /// Return the presumed column number of this location.
  //   ///
  //   /// This cannot be affected by \#line, but is packaged here for
  //   convenience. unsigned GetColumn() const {
  //     assert(IsValid());
  //     return Col;
  //   }
};

class FullSourceLocation : public SourceLocation {};

using DiagnosticInfoOrStoredDiagnotic =
    llvm::PointerUnion<const DiagnosticInfo *, const StoredDiagnostic *>;

/// DiagnosticRenderer in clang
class DiagnosticEmitter {
protected:
  const LangOptions &LangOpts;

  DiagnosticOptions &DiagOpts;

  /// The location of the previous diagnostic if known.
  ///
  /// This will be invalid in cases where there is no (known) previous
  /// diagnostic location, or that location itself is invalid or comes from
  /// a different source manager than SM.
  SourceLocation LastLoc;

  /// The level of the last diagnostic emitted.
  ///
  /// The level of the last diagnostic emitted. Used to detect level changes
  /// which change the amount of information displayed.
  DiagnosticLevel LastLevel = DiagnosticLevel::Ignored;

protected:
  DiagnosticEmitter(const LangOptions &LangOpts, DiagnosticOptions &DiagOpts);
  virtual ~DiagnosticEmitter();

protected:
  virtual void EmitDiagnosticMessage(FullSourceLocation Loc,
                                     PresumedSourceLocation PLoc,
                                     DiagnosticLevel Level,
                                     llvm::StringRef Message,
                                     ArrayRef<CharSrcRange> Ranges,
                                     DiagnosticInfoOrStoredDiagnotic Info) = 0;

  virtual void EmitDiagnosticLoc(FullSourceLocation Loc,
                                 PresumedSourceLocation PLoc,
                                 DiagnosticLevel Level,
                                 ArrayRef<CharSrcRange> Ranges) = 0;

  virtual void BeginDiagnostic(DiagnosticInfoOrStoredDiagnotic D,
                               DiagnosticLevel Level) {}

  virtual void EndDiagnostic(DiagnosticInfoOrStoredDiagnotic D,
                             DiagnosticLevel Level) {}

  virtual void EmitCodeContext(FullSourceLocation Loc, DiagnosticLevel Level,
                               llvm::SmallVectorImpl<CharSrcRange> &Ranges,
                               llvm::ArrayRef<FixIt> FixIts) = 0;
};

} // namespace diags

} // namespace stone

#endif