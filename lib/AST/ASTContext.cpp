#include "stone/AST/ASTContext.h"
#include "stone/AST/AST.h"
#include "stone/AST/Type.h"

using namespace stone;

ASTContext::ASTContext() {

  // Create VoidType
  // BuiltinFloat16Type = new (*this) Float16Type(new (*this)
  // BuiltinTypeState(*this));
}

ASTContext::~ASTContext() {}

void *stone::AllocateInASTContext(size_t bytes, const ASTContext &astContext,
                                  unsigned alignment) {
  //return astContext.AllocateMemory(bytes, alignment);
    
}
