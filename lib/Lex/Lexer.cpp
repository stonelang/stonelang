#include "stone/Parse/Lexer.h"
#include "stone/Parse/Confusable.h"

#include "clang/Basic/CharInfo.h"
#include "llvm/ADT/SmallString.h"
#include "llvm/ADT/StringSwitch.h"
#include "llvm/ADT/Twine.h"
#include "llvm/Support/MathExtras.h"
#include "llvm/Support/MemoryBuffer.h"

static bool IsWhitespace(char c) {
  return clang::isHorizontalWhitespace(c) || clang::isVerticalWhitespace(c);
}
static bool IsIdentifierStart(char c) { return clang::isIdentifierHead(c); }
static bool IsIdentifierContinue(char c) { return clang::isIdentifierBody(c); }

inline bool HasUTF8BOM(const char *Ptr) {
  return Ptr[0] == '\xEF' && Ptr[1] == '\xBB' && Ptr[2] == '\xBF';
}

Lexer::Lexer(SrcUnit &unit, SrcMgr &sm) : unit(unit), sm(sm) {

  static constexpr size_t UTF8_BOM_SIZE = 3;

  unsigned bufferID = unit.GetBufferID();
  auto buffer = sm.getBuffer(bufferID);
  assert(buffer && "Source buffer must be available in SrcMgr");

  llvm::StringRef contents = buffer->getBuffer();

  BufferStart = contents.data();
  BufferEnd = BufferStart + contents.size();
  assert(BufferStart <= BufferEnd && "Buffer is malformed");

  if (HasUTF8BOM(BufferStart)) {
    ContentStart = BufferStart + UTF8_BOM_SIZE;
  } else {
    ContentStart = BufferStart;
  }

  // Set the current lexing pointer to the start of meaningful content
  CurPtr = ContentStart;

  // Set artificial EOF to real end of file unless overridden later
  ArtificialEOF = BufferEnd;

  // Setup code completion pointer if present
  if (bufferID == sm.getCodeCompletionBufferID()) {
    const char *ptr = BufferStart + sm.getCodeCompletionOffset();
    if (ptr >= BufferStart && ptr <= BufferEnd) {
      CodeCompletionPtr = ptr;
    }
  }

  // Initialize the source location state to current position
  unsigned offset = static_cast<unsigned>(CurPtr - BufferStart);
  state.loc = SrcLoc::GetAdvanced(bufferID, offset);

  // Prime the first token
  Lex(); // emits into nextToken
}

void Lexer::Lex(Token &result) {
  result = nextToken;
  if (!result.IsEOF()) {
    Lex();
  }
}

void Lexer::Lex(TokenCallback callback) {
  while (!result.IsEOF()) {
    Token current = nextToken;
    Lex();
    callback(current);
  }
  // Emit final eof token
  callback(nextToken);
}

Lexer::Lexer(Lexer &parent, LexerState begin, LexerState end)
    : unit(parent.unit), sm(parent.sm) {

  assert(begin.IsValid() && end.IsValid());
  assert(unit.GetBufferID() == sm.findBufferContainingLoc(begin.loc) &&
         "Mismatched SrcUnit and LexerState");

  // Start lexing from the given location
  state.loc = begin.loc;

  // TODO: store or check end.loc for bounds enforcement if needed
  Lex(); // prime first token
}

void Lexer::LexUnit(SrcUnit &unit, SrcMgr &sm) {
  Lexer lexer(sm, unit);
  // lexer.Lex([&](const Token &tok) {
  //   unit.AddToken(tok);

  //   for (const DiagID &diag : lexer.ConsumeDiagnostics())
  //     unit.AddDiagnostic(diag);
  // });
}

void Lexer::Track(DiagID ID) { issues.push_back(ID); }
