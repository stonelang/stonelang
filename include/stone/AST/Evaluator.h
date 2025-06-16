#ifndef STONE_AST_DECL_ACTION_H
#define STONE_AST_DECL_ACTION_H

#include "stone/AST/ASTAllocation.h"
#include "stone/AST/InlineBitfield.h"
#include "stone/AST/TypeAlignment.h"

#include "llvm/Support/Casting.h"

#include <stdint.h>

namespace stone {

// class EvaluatorObserver {
//     virtual void WillEvaluate(EvaluatorRequest req);
//     virtual void DidEvaluate(EvaluatorRequest req, EvalResult res);
// };

enum class EvaluatorKind : uint8_t {
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

class alignas(1 << DeclAlignInBits) Evaluator
    : public ASTAllocation<Evaluator> {
  EvaluatorKind kind;

public:
  explicit Evaluator(EvaluatorKind kind) : kind(kind) {}
  // virtual ~Evaluator() = default;

public:
  EvaluatorKind GetKind() const { return kind; }
  virtual void Evaluate(Decl *decl) = 0;
};

class ShallowTypeCheckEvaluator : public Evaluator {
public:
  ShallowTypeCheckEvaluator() : Evaluator(EvaluatorKind::ShallowTypeCheck) {}
  void Evaluate(Decl *decl) override;
};

class FullTypeCheckEvaluator : public Evaluator {
public:
  FullTypeCheckEvaluator() : Evaluator(EvaluatorKind::FullTypeCheck) {}
  void Evaluate(Decl *decl) override;
};

class ShallowScaffoldingEvaluator : public Evaluator {
public:
  ShallowScaffoldingEvaluator()
      : Evaluator(EvaluatorKind::ShallowScaffolding) {}
  void Evaluate(Decl *decl) override;
};

class FullScaffoldingEvaluator : public Evaluator {
public:
  FullScaffoldingEvaluator() : Evaluator(EvaluatorKind::FullScaffolding) {}
  void Evaluate(Decl *decl) override;
};

class ShallowValidationEvaluator : public Evaluator {
public:
  ShallowValidationEvaluator() : Evaluator(EvaluatorKind::ShallowValidation) {}
  void Evaluate(Decl *decl) override;
};

class FullValidationEvaluator : public Evaluator {

public:
  FullValidationEvaluator() : Evaluator(EvaluatorKind::FullValidation) {}
  void Evaluate(Decl *decl) override;
};

class CodeGenEvaluator : public Evaluator {
public:
  CodeGenEvaluator() : Evaluator(EvaluatorKind::CodeGen) {}

public:
  virtual void Evaluate(Decl *decl) = 0;
};

} // namespace stone

#endif