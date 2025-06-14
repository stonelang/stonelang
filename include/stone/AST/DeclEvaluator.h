#ifndef STONE_AST_DECL_ACTION_H
#define STONE_AST_DECL_ACTION_H

#include "stone/AST/ASTAllocation.h"
#include "stone/AST/InlineBitfield.h"
#include "stone/AST/TypeAlignment.h"

#include "llvm/Support/Casting.h"

#include <stdint.h>

namespace stone {

enum class DeclEvaluatorKind : uint8_t {
  None = 0,

  // Type Checking
  ShallowTypeCheck,
  FullTypeCheck,

  // Scaffolding
  ShallowScaffolding,
  FullScaffolding,

  // Validation
  ShallowValidation,
  FullValidation,

  // Code Generation
  CodeGen,

  //  Desugar AliasDecl or syntactic forms

  Desugar,
  // For macro expansion or derived member injection

  Elaboration
};

class alignas(1 << DeclAlignInBits) DeclEvaluator
    : public ASTAllocation<DeclEvaluator> {
  DeclEvaluatorKind kind;

public:
  explicit DeclEvaluator(DeclEvaluatorKind kind) : kind(kind) {}
  //virtual ~DeclEvaluator() = default;

public:
  DeclEvaluatorKind GetKind() const { return kind; }
  virtual void Evaluate(Decl *decl) = 0;
};

class ShallowTypeCheckEvaluator : public DeclEvaluator {
public:
  ShallowTypeCheckEvaluator()
      : DeclEvaluator(DeclEvaluatorKind::ShallowTypeCheck) {}
  void Evaluate(Decl *decl) override;
};

class FullTypeCheckEvaluator : public DeclEvaluator {
public:
  FullTypeCheckEvaluator() : DeclEvaluator(DeclEvaluatorKind::FullTypeCheck) {}
  void Evaluate(Decl *decl) override;
};

class ShallowScaffoldingEvaluator : public DeclEvaluator {
public:
  ShallowScaffoldingEvaluator()
      : DeclEvaluator(DeclEvaluatorKind::ShallowScaffolding) {}
  void Evaluate(Decl *decl) override;
};

class FullScaffoldingEvaluator : public DeclEvaluator {
public:
  FullScaffoldingEvaluator()
      : DeclEvaluator(DeclEvaluatorKind::FullScaffolding) {}
  void Evaluate(Decl *decl) override;
};

class ShallowValidationEvaluator : public DeclEvaluator {
public:
  ShallowValidationEvaluator()
      : DeclEvaluator(DeclEvaluatorKind::ShallowValidation) {}
  void Evaluate(Decl *decl) override;
};

class FullValidationEvaluator : public DeclEvaluator {

public:
  FullValidationEvaluator()
      : DeclEvaluator(DeclEvaluatorKind::FullValidation) {}
  void Evaluate(Decl *decl) override;
};

class CodeGenEvaluator : public DeclEvaluator {
public:
  CodeGenEvaluator() : DeclEvaluator(DeclEvaluatorKind::CodeGen) {}

public:
  virtual void Evaluate(Decl *decl) = 0;
};

} // namespace stone

#endif