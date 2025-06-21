#ifndef STONE_PARSE_PARSER_H
#define STONE_PARSE_PARSER_H

#include "stone/AST/DeclState.h"
#include "stone/AST/ModuleFile.h"
#include "stone/AST/Scope.h"
#include "stone/AST/TypeState.h"
#include "stone/Parse/Lexer.h"
#include "stone/Parse/ParserResult.h"
#include "stone/Support/Token.h"

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

class ParsingDeclState final {
  Parser &parser;

  /// The DeclState passed to the Decl
  DeclState *declState;

  // The identifier we are parsing.
  Identifier identifier;

  // Direct comparison is disabled for states
  void operator==(ParsingDeclState PDS) const = delete;
  void operator!=(ParsingDeclState PDS) const = delete;

public:
  explicit ParsingDeclState(Parser &parser);

public:
  DeclState *GetDeclState() { return declState; }
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

  ModuleFile &file;
  std::unique_ptr<Lexer> lexer;

public:
  Parser(ModuleFile &file);
  ~Parser();

public:
  ModuleFile &GetFile() { return file; }
  DiagnosticEngine &GetDiags() { return file.GetDiags(); }
};

} // namespace stone

#endif