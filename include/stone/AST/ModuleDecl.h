#ifndef STONE_AST_MODULE_DECL_H
#define STONE_AST_MODULE_DECL_H

#include "stone/AST/ASTScope.h"
#include "stone/AST/ASTSession.h"
#include "stone/AST/Decl.h"
#include "stone/AST/Foreign.h"
#include "stone/AST/SourceFile.h"

#include "llvm/ADT/ArrayRef.h"
#include "llvm/ADT/SmallVector.h"

namespace stone {
class SourceFile;
class ASTSession;
class Scaffolder;
class TypeChecker;

enum class ModuleDeclKind : uint8_t {
  Normal = 0, ///< User-written Stone module (e.g., `join A;`)
  Builtin,    ///< Compiler-synthesized module (e.g., `__stone__builtin__IO`)
  Foreign     ///< External module (Clang, DWARF, etc.)
};

class ModuleDecl : public TypeDecl {

  friend Scaffolder;
  friend TypeChecker;

  ASTScope *scope = nullptr;
  llvm::SmallVector<SourceFile *, 2> sources;

  // llvm::DenseMap<Identifier, llvm::SmallVector<Decl *, 4>> symbols;

public:
  ModuleDecl(DeclState *DS);
  explicit operator bool() const { return HasFirstSourceFile(); }
  ASTScope *GetScope() const { return scope; }

public:
  SourceFile *GetFirstSourceFile() const;
  bool HasFirstSourceFile() const;
  llvm::ArrayRef<SourceFile *> GetSourceFiles() { return sources; }
  llvm::ArrayRef<const SourceFile *> GetSourceFiles() const;
  void AddSourceFile(SourceFile *sourceFile);
};

class NormalModuleDecl : public ModuleDecl {
public:
  NormalModuleDecl(DeclState *DS) : ModuleDecl(DS) {}
};

class BuiltinModuleDecl final : public ModuleDecl {
public:
  BuiltinModuleDecl(DeclState *DS) : ModuleDecl(DS) {}
};

class ForeignModuleDecl final : public ModuleDecl {
  ForeignModuleDeclKind foreignKind;

public:
  ForeignModuleDecl(DeclState *DS) : ModuleDecl(DS) {}

public:
  void SetForeignKind(ForeignModuleDeclKind kind) { foreignKind = kind; }
  ForeignModuleDeclKind GetForeignKind() { return foreignKind; }
};

} // namespace stone

#endif