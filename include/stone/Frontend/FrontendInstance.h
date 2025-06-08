#ifndef STONE_FRONTEND_FRONTENDINSTANCE_H
#define STONE_FRONTEND_FRONTENDINSTANCE_H

namespace stone {
class ModuleDecl;

class FrontendInstance final {

  /// The main module being create
  mutable ModuleDecl *mainModule = nullptr;

public:
  FrontendInstance();
};

} // namespace stone

#endif // STONE_FRONTEND_FRONTENDINPUTFILE_H
