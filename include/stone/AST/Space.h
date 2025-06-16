#ifndef STONE_AST_MODULE_DECL_H
#define STONE_AST_MODULE_DECL_H

#include "stone/AST/ASTFile.h"
#include "stone/AST/ASTScope.h"
#include "stone/AST/ASTSession.h"
#include "stone/AST/Decl.h"
#include "stone/AST/Foreign.h"

#include "llvm/ADT/ArrayRef.h"
#include "llvm/ADT/SmallVector.h"

namespace stone {
class ASTFile;
class ASTSession;
class Scaffolder;
class TypeChecker;

class SpaceParamList {
  llvm::SmallVector<Identifier, 2> subspaces;
};

class SpaceDecl : public TypeDecl {

  // friend Scaffolder;
  // friend TypeChecker;

  // ASTScope *scope = nullptr;
  // llvm::SmallVector<ASTFile *, 2> files;

  // llvm::DenseMap<Identifier, llvm::SmallVector<Decl *, 4>> symbols;

// public:
//   SpaceDecl(DeclState *DS);
//   explicit operator bool() const { return HasFirstASTFile(); }
//   ASTScope *GetScope() const { return scope; }

// public:
//   ASTFile *GetFirstASTFile() const;
//   bool HasFirstASTFile() const;
//   llvm::ArrayRef<ASTFile *> GetASTFiles() { return files; }
//   llvm::ArrayRef<const ASTFile *> GetASTFiles() const;
//   void AddASTFile(ASTFile *file);

public: SpaceDecl(DeclState *DS);
};

// class NormalSpaceDecl : public SpaceDecl {
// public:
//   NormalSpaceDecl(DeclState *DS) : SpaceDecl(DS) {}
// };

// class BuiltinSpaceDecl final : public SpaceDecl {
// public:
//   BuiltinSpaceDecl(DeclState *DS) : SpaceDecl(DS) {}
// };

// class ForeignSpaceDecl final : public SpaceDecl {
//   ForeignSpaceDeclKind foreignKind;

// public:
//   ForeignSpaceDecl(DeclState *DS) : SpaceDecl(DS) {}

// public:
//   void SetForeignKind(ForeignSpaceDeclKind kind) { foreignKind = kind; }
//   ForeignSpaceDeclKind GetForeignKind() { return foreignKind; }
// };

} // namespace stone

#endif