#include "stone/AST/Node.h"
#include "stone/AST/Decl.h"
#include "stone/AST/Expr.h"
#include "stone/AST/File.h"
#include "stone/AST/Module.h"
#include "stone/AST/Stmt.h"
#include "stone/AST/Type.h"

using namespace stone;

Node::Node() {}

void Node::AddChild(Node *child) {
  child->SetParent(this); // Ensure bidirectional connection
  children.push_back(child);
}
