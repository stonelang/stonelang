#ifndef STONE_AST_SOURCE_FILE_H
#define STONE_AST_SOURCE_FILE_H

#include "stone/AST/ASTUnit.h"
#include "stone/AST/Decl.h"

#include "llvm/ADT/SmallVector.h"

namespace stone {
class ModuleDecl;

enum class SourceFileKind : uint8_t {
  Normal = 0,     ///< A .stone file.
  Interface,      ///< A.stoneinterface file -- another module (stable ABI?)
  MacroExpansion, ///< Came from a macro expansion.
};

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

  ASTSession &GetASTSession(ModuleDecl *parent);

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
};

} // namespace stone

#endif