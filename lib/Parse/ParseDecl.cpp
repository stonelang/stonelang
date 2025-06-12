#include "stone/Parse/Parser.h"

using namespace stone;

// ParsingDeclScope::ParsingDeclScope(Parser &parser)
//     : ASTScope(parser.GetSourceFile()), parser(parser) {}

ParsingDeclState::ParsingDeclState(Parser &parser) : parser(parser) {}
