#include "stone/Parse/Parser.h"

using namespace stone;

Parser::Parser(File &file)
    : file(file), lexer(std::make_unique<Lexer>(file.GetBufferID(),
                                                file.GetDiags().GetSrcMgr())) {}
