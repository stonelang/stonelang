#include "stone/AST/Node.h"

using namespace stone;

void Node::AddChild(Node *child) {
  child->SetParent(this); // Ensure bidirectional connection
  children.push_back(child);
}
