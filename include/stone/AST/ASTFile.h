#ifndef STONE_AST_AST_FILE_H
#define STONE_AST_AST_FILE_H

#include "stone/AST/ASTFileKind.h"
#include "stone/AST/ASTUnit.h"
#include "stone/AST/Decl.h"

#include "llvm/ADT/SmallVector.h"

namespace stone {
class SpaceDecl;

enum class ASTFileStage : uint8_t {
  None = 1 << 0,
  CompletedParsing = 1 << 1,      ///< Syntax analysis
  CompletedScaffolding = 1 << 2,  ///< Semantic analysis
  CompletedTypeChecking = 1 << 3, ///< Semantic analysis
  CompletedCodeGen = 1 << 4,      ///< Generate IR
};

inline bool HasStage(ASTFileStage current, ASTFileStage check) {
  return static_cast<uint8_t>(current) & static_cast<uint8_t>(check);
}

class ASTFile final : public ASTUnit {
  unsigned bufferID;
  llvm::StringRef input;
  ASTScope *scope = nullptr;
  std::vector<Decl *> topLevelDecls;
  ASTFileStage stage = ASTFileStage::None;

public:
  ASTFile(unsigned bufferID, llvm::StringRef input, ASTSession &session);

  explicit operator bool() const {
    return HasFirstDecl() && llvm::cast<SpaceDecl>(GetFirstDecl());
  }

public:
  unsigned GetSrcBufferID() { return srcBufferID; }
  llvm::StringRef GetInput() const { return input; }

  ASTScope *GetScope() const { return scope; }
  void SetScope(ASTScope *S) { scope = S; }

  Decl *GetFirstDecl() const;
  bool HasFirstDecl() const;

  void AddTopLevelDecl(Decl *D) { topLevelDecls.push_back(D); }
  llvm::ArrayRef<Decl *> GetTopLevelDecls() const;
  ASTUnitKind GetUnitKind() const override { return ASTUnitKind::File; }

  llvm::StringRef ASTFile::GetDisplayName() const {
    return input ? input->GetName() : "<builtin>";
  }
  void Dump(llvm::raw_ostream &os) const;

public:
  static bool classof(const ASTUnit *unit) {
    return unit->GetUnitKind() == ASTUnitKind::ASTFile;
  }
};

class ASTFileList final {
  friend class ASTSession;

  llvm::SmallVector<ASTFile *, 16> files;

public:
  ASTFileList(const ASTFileList &) = delete;
  void operator=(const ASTFileList &) = delete;

public:
  ASTFileList() {}

public:
  llvm::ArrayRef<ASTFile *> GetFiles() const { return files; }
  void AddFile(ASTFile *file) { files.push_back(file); }
};
} // namespace stone

#endif