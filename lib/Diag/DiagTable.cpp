#include "stone/AST/DiagID.h"

namespace {

using namespace stone;

enum class LocalDiagnosticOptions {
  None,
  PointsToFirstBadToken,
  Fatal,
  APIDigesterBreakage,
  Deprecation,
  NoUsage
};

struct LocalDiagnosticInfo {
  DiagnosticKind kind : 2;
  bool pointsToFirstBadToken : 1;
  bool isFatal : 1;
  bool isAPIDigesterBreakage : 1;
  bool isDeprecation : 1;
  bool isNoUsage : 1;

  constexpr LocalDiagnosticInfo(DiagnosticKind k, LocalDiagnosticOptions opts)
      : kind(k), pointsToFirstBadToken(
                     opts == LocalDiagnosticOptions::PointsToFirstBadToken),
        isFatal(opts == LocalDiagnosticOptions::Fatal),
        isAPIDigesterBreakage(opts ==
                              LocalDiagnosticOptions::APIDigesterBreakage),
        isDeprecation(opts == LocalDiagnosticOptions::Deprecation),
        isNoUsage(opts == LocalDiagnosticOptions::NoUsage) {}
};

enum LocalDiagID : uint32_t {
#define DIAG(KIND, ID, Options, Text, Signature) ID,
#include "stone/Core/DiagEngine.def"
  NumDiags
};

static constexpr LocalDiagnosticInfo storedDiagnosticInfos[] = {
#define ERROR(ID, Options, Text, Signature)                                    \
  LocalDiagnosticInfo(DiagnosticKind::Error, LocalDiagnosticOptions::Options),
#define WARNING(ID, Options, Text, Signature)                                  \
  LocalDiagnosticInfo(DiagnosticKind::Warning, LocalDiagnosticOptions::Options),
#define NOTE(ID, Options, Text, Signature)                                     \
  LocalDiagnosticInfo(DiagnosticKind::Note, LocalDiagnosticOptions::Options),
#define REMARK(ID, Options, Text, Signature)                                   \
  LocalDiagnosticInfo(DiagnosticKind::Remark, LocalDiagnosticOptions::Options),
#include "stone/Core/DiagEngine.def"
};

static_assert(static_cast<uint32_t>(LocalDiagID::NumDiags) ==
                  sizeof(storedDiagnosticInfos) / sizeof(LocalDiagnosticInfo),
              "Mismatch between diagnostic enum and info table");

static constexpr const char *diagnosticTexts[] = {
#define DIAG(KIND, ID, Options, Text, Signature) Text,
#include "stone/Core/DiagEngine.def"
    "<not a diagnostic>"};

static constexpr const char *debugDiagnosticTexts[] = {
#define DIAG(KIND, ID, Options, Text, Signature) Text " [" #ID "]",
#include "stone/Core/DiagEngine.def"
    "<not a diagnostic>"};

static constexpr const char *diagnosticIDNames[] = {
#define DIAG(KIND, ID, Options, Text, Signature) #ID,
#include "stone/Core/DiagEngine.def"
    "<not a diagnostic>"};

static constexpr const char *fixItTexts[] = {
#define DIAG(KIND, ID, Options, Text, Signature)
#define FIXIT(ID, Text, Signature) Text,
#include "stone/Core/DiagEngine.def"
    "<not a fix-it>"};

} // namespace
