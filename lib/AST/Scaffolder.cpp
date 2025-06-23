#include "stone/AST/Scaffolder.h"
#include "stone/AST/Decl.h"
#include "stone/AST/EvaluatorEngine.h"

using namespace stone;

Scaffolder::Scaffolder() {}

void Scaffolder::ScaffoldDecl(Decl *D) {}

void Scaffolder::ScaffoldSpaceDecl(SpaceDecl *MD) {}

void FullScaffoldingEvaluator::Evaluate(Decl *D) {
  // D->GetFlight()->GetASTSession().CreateScafolder().ScaffoldSpaceDecl(D);
}

void ShallowValidationEvaluator::Evaluate(Decl *D) {}
