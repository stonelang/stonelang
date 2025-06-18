#include "stone/Parse/Parser.h"

using namespace stone;

// ParsingDeclScope::ParsingDeclScope(Parser &parser)
//     : Scope(parser.GetSourceFile()), parser(parser) {}

ParsingDeclState::ParsingDeclState(Parser &parser) : parser(parser) {}
