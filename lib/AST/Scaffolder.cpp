#include "stone/AST/Scaffolder.h"
#include "stone/AST/ModuleDecl.h"
#include "stone/AST/DeclEvaluator.h"


using namespace stone;

Scaffolder::Scaffolder() {}

void Scaffolder::ScaffoldDecl(Decl *D) {}

void Scaffolder::ScaffoldModuleDecl(ModuleDecl *MD) {}

void FullScaffoldingEvaluator::Evaluate(Decl *D) {
  // D->GetState()->GetASTSession().CreateScafolder().ScaffoldModuleDecl(D);
}

void ShallowValidationEvaluator::Evaluate(Decl *D) {}
