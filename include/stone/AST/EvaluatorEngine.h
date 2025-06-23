#ifndef STONE_AST_DECL_ACTION_H
#define STONE_AST_DECL_ACTION_H

#include "stone/AST/Alignments.h"
#include "stone/AST/Allocation.h"
#include "stone/AST/InlineBitfield.h"

#include "llvm/Support/Casting.h"

#include <stdint.h>

namespace stone {

// class EvaluatorObserver {
//     virtual void WillEvaluate(EvaluatorRequest req);
//     virtual void DidEvaluate(EvaluatorRequest req, EvalResult res);
// };

enum class EvaluatorKind : uint8_t {

  Syntax = 0,

  // Scaffolding
  ShallowScaffolding,
  FullScaffolding,

  // Type Checking
  ShallowLogic,
  FullLogic,

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

class alignas(1 << DeclAlignInBits) Evaluator : public Allocation<Evaluator> {
  EvaluatorKind kind;

public:
  explicit Evaluator(EvaluatorKind kind) : kind(kind) {}
  // virtual ~Evaluator() = default;

public:
  EvaluatorKind GetKind() const { return kind; }
  virtual void Evaluate(Decl *decl) = 0;
};

class SyntaxEvaluator : public Evaluator {
public:
  SyntaxEvaluator() : Evaluator(EvaluatorKind::Syntax) {}
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

class ShallowLogicEvaluator : public Evaluator {
public:
  ShallowLogicEvaluator() : Evaluator(EvaluatorKind::ShallowLogic) {}
  void Evaluate(Decl *decl) override;
};

class FullLogicEvaluator : public Evaluator {
public:
  FullLogicEvaluator() : Evaluator(EvaluatorKind::FullLogic) {}
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

// struct EvaluatorRequest {
//   Tree *decl = nullptr;
//   EvaluatorKind kind;

//   EvaluatorRequest(Decl *decl, EvaluatorKind kind)
//       : decl(decl), kind(kind) {}
// };

template <typename T> class EvaluatorRequest final {
public:
  EvaluatorKind kind;
  T *val;
  EvaluatorRequest(EvaluatorKind k, T *v) : kind(k), val(v) {}

  bool cacheable() const { return true; }
};
class EvaluatorEngine final {

public:
  EvaluatorEngine();

  // llvm::DenseMap<std::pair<DeclOrFile *, EvaluatorKind>, EvalResult> results;

  // llvm::DenseMap<EvaluatorKind, std::vector<EvaluatorKind>> pipeline = {
  //   {EvaluatorKind::CodeGen, {
  //       EvaluatorKind::ShallowScaffolding,
  //       EvaluatorKind::FullLogic,
  //       EvaluatorKind::Elaboration,
  //       EvaluatorKind::CodeGen
  //   }},
  //   {EvaluatorKind::FullLogic, {
  //       EvaluatorKind::ShallowScaffolding,
  //       EvaluatorKind::FullLogic
  //   }},
  //   {EvaluatorKind::Syntax, {EvaluatorKind::Syntax}},
  //   // more as needed...
  // };

  // std::vector<std::unique_ptr<Evaluator>> evaluators;
  // EvaluatorObserver *observer = nullptr;
};

} // namespace stone

#endif