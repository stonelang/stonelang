#include "stone/AST/Visibility.h"

using namespace stone;

/// At a high level, this checks the given declaration's signature does not
/// reference any other declarations that are less visible than the
/// declaration itself. Related checks may also be performed.
void stone::CheckVisibility(Decl *D) {}

/// Check the Type visibility level
void stone::CheckVisibility(Type *ty) {}
