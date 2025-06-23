#ifndef STONE_AST_STMT_H
#define STONE_AST_STMT_H

#include "stone/AST/Node.h"
#include "stone/AST/NodeKind.h"
#include "stone/AST/StmtKind.h"

namespace stone {
class StmtFlight;

/// \brief The base class for all statement nodes in the AST.
///
/// Each `Stmt` wraps a specific `StmtKind` and participates in the `Node`
/// hierarchy using `NodeKind::Stmt`. Subclasses are defined in `StmtNode.def`
/// via macro expansion, and are declared final for dispatch clarity.
///
/// This design allows each statement to be tagged with a `StmtKind` and
/// extended using flight metadata (`StmtFlight*`) if desired.
class alignas(8) Stmt : public Node {

  StmtFlight *flight = nullptr;

public:
  /// \brief Constructs a Stmt with a specific kind.
  explicit Stmt(StmtFlight *flight);

  /// \brief Returns the kind of this statement.
  StmtKind GetKind() const;

  /// \brief LLVM-style RTTI: Returns true if this is a Stmt.
  static bool classof(const Stmt *stmt) {
    // return stmt->GetKind() >= LastStmt && stmt->GetKind() <= FirstStmt;
    return true;
  }

  /// \brief LLVM-style RTTI: Returns true if the node is a Stmt.
  static bool classof(const Node *node) {
    return node->GetKind() == NodeKind::Stmt;
  }
};

/// \brief Defines a concrete Stmt subclass corresponding to each StmtKind.
///
/// This macro generates a final class for every statement kind defined in
/// `StmtNode.def`. Each subclass wraps a `StmtFlight*`, passed to the base
/// `Stmt` constructor. This allows easy access to kind and metadata, while
/// supporting LLVM-style RTTI.
///
/// Example Expansion:
/// \code
/// class ReturnStmt final : public Stmt {
/// public:
///   explicit ReturnStmt(StmtFlight *flight) : Stmt(flight) {}
///   static bool classof(const Stmt *S);
/// };
/// \endcode
#define STMT(ID, Parent)                                                       \
  class ID##Stmt final : public Parent {                                       \
  public:                                                                      \
    explicit ID##Stmt(StmtFlight *flight) : Parent(flight) {}                  \
    static bool classof(const Stmt *S);                                        \
  };

#include "stone/AST/StmtNode.def"

} // namespace stone

#endif // STONE_AST_STMT_H
