#include "stone/AST/ASTContext.h"
#include "stone/AST/Type.h"

using namespace stone;

ASTContext::ASTContext() {

  // Create VoidType
  // BuiltinFloat16Type = new (*this) Float16Type(new (*this)
  // BuiltinTypeState(*this));
}

ASTContext::~ASTContext() {}