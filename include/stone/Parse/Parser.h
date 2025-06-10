#ifndef STONE_PARSE_PARSER_H
#define STONE_PARSE_PARSER_H

#include "stone/AST/AST.h"
#include "stone/AST/DeclState.h"
#include "stone/AST/TypeState.h"
#include "stone/Lang/Token.h"
#include "stone/Parse/ParserResult.h"

namespace stone {
class Parser;

/// To assist debugging parser crashes, tell us the location of the
/// current curTok.
class ParsingPrettyStackTrace final : public llvm::PrettyStackTraceEntry {
  Parser &parser;

public:
  explicit ParsingPrettyStackTrace(Parser &parser);
  void print(llvm::raw_ostream &out) const override;
};

class ParsingASTScope final : public ASTScope {
  Parser &currentParser;

public:
  ParsingASTScope(const ParsingASTScope &) = delete;
  void operator=(const ParsingASTScope &) = delete;

public:
  ParsingASTScope(Parser &currentParser, ASTScopeKind kind);
  ~ParsingASTScope();

public:
  Parser &GetCurrentParser() { return currentParser; }
};

class ParsingDeclOptions final {
  unsigned storage;

public:
  /// Flags that control the parsing of declarations.
  enum Flags {
    None = 0,
    AllowTopLevel = 1 << 1,
    InTopLevel = 1 << 2,
    HasContainerType = 1 << 3,
    AllowDestructor = 1 << 4,
    AllowEnumElement = 1 << 5,
    InInterface = 1 << 6,
    InStruct = 1 << 7,
    InEnum = 1 << 8,
  };

public:
  ParsingDeclOptions() {}

public:
  void AddAllowTopLevel() { storage |= Flags::AllowTopLevel; }
  bool HasAllowTopLevel() const { return storage & Flags::AllowTopLevel; }
  ///\Has only AllowTopLevel
  bool IsAllowTopLevelOnly() const { return storage == Flags::AllowTopLevel; }
  ///\Clear AllowTopLevel
  void ClearAllowTopLevel() { storage &= ~Flags::AllowTopLevel; }

public:
};

class Parser final {
public:
};

} // namespace stone

#endif