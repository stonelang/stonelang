#include "stone/AST/ASTFile.h"
#include "stone/AST/ASTSession.h"
#include "stone/AST/SpaceDecl.h"

using namespace stone;

ASTFile::ASTFile(unsigned bufferID, llvm::StringRef input, ASTSession &session)
    : ASTUnit(session), kind(kind), bufferID(bufferID) {}

Decl *ASTFile::GetFirstDecl() const { return nullptr; }
bool ASTFile::HasFirstDecl() const { return GetFirstDecl() != nullptr; }

llvm::ArrayRef<Decl *> ASTFile::GetTopLevelDecls() const {
  return topLevelDecls;
}

ASTFile *ASTSession::CreateASTFile(unsigned bufferID, llvm::StringRef input,
                                   SpaceDecl *parent) {
  return nullptr;
}
