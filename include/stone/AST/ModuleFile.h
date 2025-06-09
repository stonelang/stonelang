#ifndef STONE_AST_MODULE_FILE_H
#define STONE_AST_MODULE_FILE_H

#include "stone/AST/AST.h"
#include "stone/AST/Decl.h"
#include "stone/AST/DeclContext.h"

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

class ModuleFile : public DeclContext, public ASTAllocation<ModuleFile> {
private:
  ModuleFileKind kind;

public:
  ModuleFile(ModuleFileKind kind, ModuleDecl *owner);

public:
  bool IsSource() const { return kind == ModuleFileKind::Source; }
  bool IsBuiltin() const { return kind == ModuleFileKind::Builtin; }
  bool IsClangModule() const { return kind == ModuleFileKind::ClangModule; }
};

enum class SourceFileStage : uint8_t {
  None = 1 << 0,
  Parsed = 1 << 1,
  TypeChecked = 1 << 2,
  EmittedIR = 1 << 3,
};

class SourceFile final : public ModuleFile {

  std::vector<Decl *> topLevelDecls;

public:
  SourceFile();
  explicit operator bool() const {
    return HasFirstDecl() && llvm::cast<JoinDecl>(GetFirstDecl());
  }

public:
  Decl *GetFirstDecl() const;
  bool HasFirstDecl() const;

public:
  void AddTopLevelDecl(Decl *D) { topLevelDecls.push_back(D); }
  llvm::ArrayRef<Decl *> GetTopLevelDecls() const;
};

class BuiltinFile final : public ModuleFile {
public:
};

} // namespace stone

#endif