#ifndef STONE_PARSE_PARSER_H
#define STONE_PARSE_PARSER_H

#include "stone/AST/DeclFlight.h"
#include "stone/AST/File.h"
#include "stone/AST/Scope.h"
#include "stone/AST/TypeFlight.h"
#include "stone/Core/Token.h"
#include "stone/Parse/Lexer.h"
#include "stone/Parse/ParserResult.h"

#include "llvm/Support/PrettyStackTrace.h"

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

class ParsingDeclFlight final {
  Parser &parser;

  /// The DeclFlight passed to the Decl
  DeclFlight *declState;

  // The identifier we are parsing.
  Identifier identifier;

  // Direct comparison is disabled for states
  void operator==(ParsingDeclFlight PDS) const = delete;
  void operator!=(ParsingDeclFlight PDS) const = delete;

public:
  explicit ParsingDeclFlight(Parser &parser);

public:
  DeclFlight *GetDeclFlight() { return declState; }
  void SetIdentifier(Identifier identifier) { identifier = identifier; }
  Identifier GetDeclIdentifier() { return identifier; }
  Parser &GetParser() { return parser; }

  TypeInfluencerList &GetTypeInfluencerList() {
    return declState->GetTypeInfluencerList();
  }
  DeclInfluencerList &GetDeclInfluencerList() {
    return declState->GetDeclInfluencerList();
  }
};
class Parser final {

  File &file;
  std::unique_ptr<Lexer> lexer;

public:
  Parser(File &file);
  ~Parser();

public:
  File &GetFile() { return file; }
  DiagnosticEngine &GetDiags() { return file.GetDiags(); }
};

} // namespace stone

#endif