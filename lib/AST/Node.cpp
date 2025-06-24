#include "stone/AST/Node.h"
#include "stone/AST/Decl.h"
#include "stone/AST/Expr.h"
#include "stone/AST/File.h"
#include "stone/AST/Module.h"
#include "stone/AST/Stmt.h"
#include "stone/AST/Type.h"

using namespace stone;

Node::Node(NodeUnion parent) : parent(parent) {}

NodeKind Node::GetKind() const {
  if (parent.is<Decl *>())
    return NodeKind::Decl;
  if (parent.is<Expr *>())
    return NodeKind::Expr;
  if (parent.is<Stmt *>())
    return NodeKind::Stmt;
  if (parent.is<Type *>())
    return NodeKind::Type;
  if (parent.is<File *>())
    return NodeKind::File;
  if (parent.is<Module *>())
    return NodeKind::Module;
  llvm_unreachable("Unknown node kind");
}