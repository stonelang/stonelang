#ifndef STONE_COMPILE_COMPILE_H
#define STONE_COMPILE_COMPILE_H

namespace stone {
class FrontendObserver;
int Compile(llvm::ArrayRef<const char *> args, const char *arg0, void *mainAddr,
            FrontendObserver *observer = nullptr);
} // namespace stone

#endif
