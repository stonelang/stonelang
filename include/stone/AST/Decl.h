#ifndef STONE_AST_DECL_H
#define STONE_AST_DECL_H

#include "stone/AST/DeclKind.h"
#include "stone/AST/Identifier.h"
#include "stone/AST/Node.h"
#include "stone/AST/NodeKind.h"
#include "stone/AST/Alignments.h"
#include "stone/Core/SrcLoc.h"

#include "llvm/Support/Casting.h"

#include <stdint.h>

namespace stone {

class DeclFlight;
/// \brief Base class for all declaration nodes in the AST.
///
/// Decl nodes provide lightweight navigation and identification.
/// All rich metadata and semantic information is stored externally
/// in the associated DeclFlight, which is accessible via GetFlight().
///
/// Decl is aligned according to DeclAlignInBits for efficient packing.
class alignas(1 << DeclAlignInBits) Decl : public Node<NodeKind::Decl, Decl> {

  using Base = Node<NodeKind::Decl, Decl>;
  /// Pointer to the associated metadata record for this declaration.
  /// Carries all semantic attributes, modifiers, and type info.
  DeclFlight *flight = nullptr;

public:
  using Base::Base;

  /// \brief Constructs a Decl with a reference to its metadata.
  explicit Decl(DeclFlight *flight);

  // Inherit the constructor
  // using Node::Node;

  /// \returns The associated metadata object for this declaration.
  DeclFlight *GetFlight();

  /// \returns The metadata object cast to the given type.
  /// Usage assumes caller knows the concrete subclass of the flight.
  template <typename T> T *GetFlightAs() { return llvm::cast<T>(flight); }

  //===--------------------------------------------------------------------===//
  // Core Identification
  // These provide the minimal identifying data for AST traversal and
  // diagnostics.
  //===--------------------------------------------------------------------===//

  /// \returns The specific kind of declaration (e.g., Fun, Struct, Var).
  DeclKind GetKind() const;

  /// \returns The declared name identifier (may be empty for unnamed decls).
  Identifier GetName() const;

  /// \returns The source location of the name token for diagnostics and
  /// mapping.
  SrcLoc GetLoc() const;

public:
  /// \returns true if the given Decl pointer is of this base type.
  static bool classof(const Decl *D);

  /// \returns true if the given Artifact is a Decl.
  static bool classof(const Node *N);
};

/// \brief Defines a concrete Decl subclass corresponding to each DeclKind.
///
/// This macro generates a final class for every declaration kind defined in
/// DeclNode.def. Each subclass wraps a `DeclFlight*`, passed to the base
/// `Decl` constructor. The macro also declares a `classof` method for use
/// with LLVM-style RTTI.
///
/// Example Expansion:
/// \code
/// class FunDecl final : public Decl {
/// public:
///   explicit FunDecl(DeclFlight *flight) : Decl(flight) {}
///   static bool classof(const Decl *D);
/// };
/// \endcode
#define DECL(ID, Parent)                                                       \
  class ID##Decl final : public Parent {                                       \
  public:                                                                      \
    explicit ID##Decl(DeclFlight *flight) : Parent(flight) {}                  \
    static bool classof(const Decl *D);                                        \
  };

#include "stone/AST/DeclNode.def"

} // namespace stone

#endif
