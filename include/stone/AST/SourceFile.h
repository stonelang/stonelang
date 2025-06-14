#ifndef STONE_AST_SOURCE_FILE_H
#define STONE_AST_SOURCE_FILE_H

#include "stone/AST/ASTUnit.h"
#include "stone/AST/Decl.h"
#include "stone/AST/SourceFileKind.h"

#include "llvm/ADT/SmallVector.h"

namespace stone {
class ModuleDecl;

enum class SourceFileStage : uint8_t {
  None = 1 << 0,
  CompletedParsing = 1 << 1,   ///< Syntax analysis
  CompletedSemantics = 1 << 2, ///< Semantic analysis
  CompletedCodeGen = 1 << 3,   ///< Generate IR
};

class SourceFile final : public ASTUnit {

  SourceFileKind kind;
  unsigned srcBufferID;
  /// May way ASTUnit
  std::vector<Decl *> topLevelDecls;

public:
  SourceFile(SourceFileKind kind, unsigned srcBufferID, ModuleDecl *parent);
  explicit operator bool() const {
    return HasFirstDecl() && llvm::cast<JoinDecl>(GetFirstDecl());
  }

public:
  SourceFileKind GetSourceFileKind() { return kind; }
  unsigned GetSrcBufferID() { return srcBufferID; }

  Decl *GetFirstDecl() const;
  bool HasFirstDecl() const;
  void AddTopLevelDecl(Decl *D) { topLevelDecls.push_back(D); }
  llvm::ArrayRef<Decl *> GetTopLevelDecls() const;
  ASTUnitKind GetUnitKind() const override { return ASTUnitKind::SourceFile; }

public:
  static bool classof(const ASTUnit *unit) {
    return unit->GetUnitKind() == ASTUnitKind::SourceFile;
  }
};

} // namespace stone

#endif