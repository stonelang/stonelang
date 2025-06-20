#ifndef STONE_AST_MODULEFILE_H
#define STONE_AST_MODULEFILE_H

#include "stone/AST/Decl.h"
#include "stone/AST/Node.h"
// #include "stone/AST/FileArtifact.h"
#include "stone/AST/Scope.h"

#include "llvm/ADT/SmallVector.h"

namespace stone {
class Module;
class SpaceDecl;
enum class ModuleFileStage : uint8_t {
  None = 1 << 0,
  CompletedParsing = 1 << 1,      ///< Syntax analysis
  CompletedScaffolding = 1 << 2,  ///< Semantic analysis
  CompletedTypeChecking = 1 << 3, ///< Semantic analysis
  CompletedCodeGen = 1 << 4,      ///< Generate IR
};

inline bool HasStage(ModuleFileStage current, ModuleFileStage check) {
  return static_cast<uint8_t>(current) & static_cast<uint8_t>(check);
}

class ModuleFile final : public Node {
  unsigned bufferID;
  llvm::StringRef input;
  Scope *scope = nullptr;
  std::vector<Decl *> topLevelDecls;
  ModuleFileStage stage = ModuleFileStage::None;

public:
  ModuleFile(unsigned bufferID, llvm::StringRef input, Module *parent);

  explicit operator bool() const {
    return HasFirstDecl() && llvm::cast<SpaceDecl>(GetFirstDecl());
  }

public:
  unsigned GetSrcBufferID() { return bufferID; }
  llvm::StringRef GetInput() const { return input; }

  Scope *GetScope() const { return scope; }
  void SetScope(Scope *S) { scope = S; }

  Decl *GetFirstDecl() const;
  bool HasFirstDecl() const;

  void AddTopLevelDecl(Decl *D) { topLevelDecls.push_back(D); }
  llvm::ArrayRef<Decl *> GetTopLevelDecls() const;

  ArtifactKind GetArtifactKind() const override {
    return ArtifactKind::ModuleFile;
  }
  // void Flush() override;

  llvm::StringRef GetInputName() { return input; }
  // llvm::StringRef ModuleFile::GetDisplayName() const {
  //   return input ? input->GetName() : "<builtin>";
  // }
  void Dump(llvm::raw_ostream &os) const;

public:
  static bool classof(const Artifact *unit) {
    return unit->GetArtifactKind() == ArtifactKind::ModuleFile;
  }
};

} // namespace stone

#endif