#ifndef STONE_AST_MODULE_FILE_H
#define STONE_AST_MODULE_FILE_H

#include "stone/AST/ASTUnit.h"
#include "stone/AST/Decl.h"

#include "llvm/ADT/SmallVector.h"

namespace stone {
class ModuleDecl;

enum class ModuleFileKind : uint8_t {
  /// .stone file
  Source,
  /// Compiler builtins
  Builtin,
  /// Imported clang module.
  ClangModule
};

class ModuleFile : public ASTUnit {
  ModuleFileKind kind;
  ASTSession &GetASTSession(ModuleDecl *parent);

public:
  ModuleFile(ModuleFileKind kind, ModuleDecl *parent);

public:
  ASTUnitKind GetUnitKind() const override { return ASTUnitKind::ModuleFile; }
  bool IsSource() const { return kind == ModuleFileKind::Source; }
  bool IsBuiltin() const { return kind == ModuleFileKind::Builtin; }
  bool IsClangModule() const { return kind == ModuleFileKind::ClangModule; }
};

enum class SourceFileKind : uint8_t {
  Library = 0, ///< A normal .swift file.
  Main,        ///< A .swift file that can have top-level code.
  Interface, ///< Came from a .swiftinterface file, representing another module.
  MacroExpansion, ///< Came from a macro expansion.
};

enum class SourceFileStage : uint8_t {
  None = 1 << 0,
  Parsed = 1 << 1,
  TypeChecked = 1 << 2,
  EmittedIR = 1 << 3,
};

class SourceFile final : public ModuleFile {

  SourceFileKind kind;
  unsigned srcBufferID;
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
};

class BuiltinFile final : public ModuleFile {
public:
};

} // namespace stone

#endif