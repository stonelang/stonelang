#include "stone/Frontend/FrontendOptions.h"

using namespace stone;

FrontendAction::FrontendAction(FrontendActionKind kind) : kind(kind) {}

FrontendOptions::FrontendOptions() : action(FrontendActionKind::None) {}
