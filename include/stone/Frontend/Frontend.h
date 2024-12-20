#ifndef STONE_FRONTEND_FRONTEND_H
#define STONE_FRONTEND_FRONTEND_H

#include <memory>
namespace stone {

class FrontendInvocation {
public:
	FrontendInvocation();
};

class FrontendAction {
public:
	FrontendAction();
};

class FrontendInstance {
public:
	std::unique_ptr<FrontendInvocation> invocation;
public:
	
};

} // namespace stone

#endif
