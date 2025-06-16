#include "stone/AST/Scaffolder.h"
#include "stone/AST/Evaluator.h"
#include "stone/AST/SpaceDecl.h"

using namespace stone;

Scaffolder::Scaffolder() {}

void Scaffolder::ScaffoldDecl(Decl *D) {}

void Scaffolder::ScaffoldSpaceDecl(SpaceDecl *MD) {}

void FullScaffoldingEvaluator::Evaluate(Decl *D) {
  // D->GetState()->GetASTSession().CreateScafolder().ScaffoldSpaceDecl(D);
}

void ShallowValidationEvaluator::Evaluate(Decl *D) {}
