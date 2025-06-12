#ifndef STONE_AST_MODULE_DECL_H
#define STONE_AST_MODULE_DECL_H

#include "stone/AST/ASTScope.h"
#include "stone/AST/ASTSession.h"
#include "stone/AST/Decl.h"
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

  ModuleDeclKind kind;
  ASTScope *scope = nullptr;
  llvm::SmallVector<SourceFile *, 2> sources;
  // llvm::DenseMap<Identifier, llvm::SmallVector<Decl *, 4>> symbols;

public:
  ModuleDecl(ModuleDeclKind kind, ASTSession &session);
  explicit operator bool() const { return HasFirstSourceFile(); }
  ModuleDeclKind GetModuleKind() const { return kind; }
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
  NormalModuleDecl(ASTSession &session)
      : ModuleDecl(ModuleDeclKind::Normal, session) {}
};

class BuiltinModuleDecl : public ModuleDecl {
public:
  BuiltinModuleDecl(ASTSession &session)
      : ModuleDecl(ModuleDeclKind::Builtin, session) {}
};

enum class ForeignModuleDeclKind : uint8_t {
  Clang = 0, ///< Imported via Clang bridge
  DWARF      ///< Possibly from debug symbols
};

class ForeignModuleDecl : public ModuleDecl {
  ForeignModuleDeclKind kind;

public:
  ForeignModuleDecl(ForeignModuleDeclKind kind, ASTSession &session)
      : ModuleDecl(ModuleDeclKind::Foreign, session), kind(kind) {}

public:
  ForeignModuleDeclKind GetForeignModuleKind() const { return kind; }
};

} // namespace stone

#endif