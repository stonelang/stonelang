//===- TableGen.cpp - Top-Level TableGen implementation for Stone ---------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file contains the main function for Stone's TableGen.
//
//===----------------------------------------------------------------------===//

#include "TableGenBackends.h" // Declares all backends.
#include "ASTTableGen.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/ManagedStatic.h"
#include "llvm/Support/PrettyStackTrace.h"
#include "llvm/Support/Signals.h"
#include "llvm/TableGen/Error.h"
#include "llvm/TableGen/Main.h"
#include "llvm/TableGen/Record.h"

using namespace llvm;
using namespace stone;

enum ActionType {
  PrintRecords,
  DumpJSON,
  GenStoneAttrClasses,
  GenStoneAttrParserStringSwitches,
  GenStoneAttrSubjectMatchRulesParserStringSwitches,
  GenStoneAttrImpl,
  GenStoneAttrList,
  GenStoneAttrDocTable,
  GenStoneAttrSubjectMatchRuleList,
  GenStoneAttrPCHRead,
  GenStoneAttrPCHWrite,
  GenStoneRegularKeywordAttributeInfo,
  GenStoneAttrHasAttributeImpl,
  GenStoneAttrSpellingListIndex,
  GenStoneAttrASTVisitor,
  GenStoneAttrTemplateInstantiate,
  GenStoneAttrParsedAttrList,
  GenStoneAttrParsedAttrImpl,
  GenStoneAttrParsedAttrKinds,
  GenStoneAttrTextNodeDump,
  GenStoneAttrNodeTraverse,
  GenStoneBasicReader,
  GenStoneBasicWriter,
  GenStoneBuiltins,
  GenStoneDiagsDefs,
  GenStoneDiagGroups,
  GenStoneDiagsIndexName,
  GenStoneCommentNodes,
  GenStoneDeclNodes,
  GenStoneStmtNodes,
  GenStoneTypeNodes,
  GenStoneTypeReader,
  GenStoneTypeWriter,
  GenStoneOpcodes,
  GenStoneSACheckers,
  GenStoneSyntaxNodeList,
  GenStoneSyntaxNodeClasses,
  GenStoneCommentHTMLTags,
  GenStoneCommentHTMLTagsProperties,
  GenStoneCommentHTMLNamedCharacterReferences,
  GenStoneCommentCommandInfo,
  GenStoneCommentCommandList,
  GenStoneOpenCLBuiltins,
  GenStoneOpenCLBuiltinHeader,
  GenStoneOpenCLBuiltinTests,
  GenArmNeon,
  GenArmFP16,
  GenArmBF16,
  GenArmVectorType,
  GenArmNeonSema,
  GenArmNeonTest,
  GenArmMveHeader,
  GenArmMveBuiltinDef,
  GenArmMveBuiltinSema,
  GenArmMveBuiltinCG,
  GenArmMveBuiltinAliases,
  GenArmSveHeader,
  GenArmSveBuiltins,
  GenArmSveBuiltinCG,
  GenArmSveTypeFlags,
  GenArmSveRangeChecks,
  GenArmSveStreamingAttrs,
  GenArmSmeHeader,
  GenArmSmeBuiltins,
  GenArmSmeBuiltinCG,
  GenArmSmeRangeChecks,
  GenArmSmeStreamingAttrs,
  GenArmSmeBuiltinZAState,
  GenArmCdeHeader,
  GenArmCdeBuiltinDef,
  GenArmCdeBuiltinSema,
  GenArmCdeBuiltinCG,
  GenArmCdeBuiltinAliases,
  GenRISCVVectorHeader,
  GenRISCVVectorBuiltins,
  GenRISCVVectorBuiltinCG,
  GenRISCVVectorBuiltinSema,
  GenRISCVSiFiveVectorBuiltins,
  GenRISCVSiFiveVectorBuiltinCG,
  GenRISCVSiFiveVectorBuiltinSema,
  GenAttrDocs,
  GenDiagDocs,
  GenOptDocs,
  GenDataCollectors,
  GenTestPragmaAttributeSupportedAttributes
};

namespace {
cl::opt<ActionType> Action(
    cl::desc("Action to perform:"),
    cl::values(
        clEnumValN(PrintRecords, "print-records",
                   "Print all records to stdout (default)"),
        clEnumValN(DumpJSON, "dump-json",
                   "Dump all records as machine-readable JSON"),
        clEnumValN(GenStoneAttrClasses, "gen-stone-attr-classes",
                   "Generate stone attribute clases"),
        clEnumValN(GenStoneAttrParserStringSwitches,
                   "gen-stone-attr-parser-string-switches",
                   "Generate all parser-related attribute string switches"),
        clEnumValN(GenStoneAttrSubjectMatchRulesParserStringSwitches,
                   "gen-stone-attr-subject-match-rules-parser-string-switches",
                   "Generate all parser-related attribute subject match rule"
                   "string switches"),
        clEnumValN(GenStoneAttrImpl, "gen-stone-attr-impl",
                   "Generate stone attribute implementations"),
        clEnumValN(GenStoneAttrList, "gen-stone-attr-list",
                   "Generate a stone attribute list"),
        clEnumValN(GenStoneAttrDocTable, "gen-stone-attr-doc-table",
                   "Generate a table of attribute documentation"),
        clEnumValN(GenStoneAttrSubjectMatchRuleList,
                   "gen-stone-attr-subject-match-rule-list",
                   "Generate a stone attribute subject match rule list"),
        clEnumValN(GenStoneAttrPCHRead, "gen-stone-attr-pch-read",
                   "Generate stone PCH attribute reader"),
        clEnumValN(GenStoneAttrPCHWrite, "gen-stone-attr-pch-write",
                   "Generate stone PCH attribute writer"),
        clEnumValN(GenStoneRegularKeywordAttributeInfo,
                   "gen-stone-regular-keyword-attr-info",
                   "Generate a list of regular keyword attributes with info "
                   "about their arguments"),
        clEnumValN(GenStoneAttrHasAttributeImpl,
                   "gen-stone-attr-has-attribute-impl",
                   "Generate a stone attribute spelling list"),
        clEnumValN(GenStoneAttrSpellingListIndex,
                   "gen-stone-attr-spelling-index",
                   "Generate a stone attribute spelling index"),
        clEnumValN(GenStoneAttrASTVisitor, "gen-stone-attr-ast-visitor",
                   "Generate a recursive AST visitor for stone attributes"),
        clEnumValN(GenStoneAttrTemplateInstantiate,
                   "gen-stone-attr-template-instantiate",
                   "Generate a stone template instantiate code"),
        clEnumValN(GenStoneAttrParsedAttrList,
                   "gen-stone-attr-parsed-attr-list",
                   "Generate a stone parsed attribute list"),
        clEnumValN(GenStoneAttrParsedAttrImpl,
                   "gen-stone-attr-parsed-attr-impl",
                   "Generate the stone parsed attribute helpers"),
        clEnumValN(GenStoneAttrParsedAttrKinds,
                   "gen-stone-attr-parsed-attr-kinds",
                   "Generate a stone parsed attribute kinds"),
        clEnumValN(GenStoneAttrTextNodeDump, "gen-stone-attr-text-node-dump",
                   "Generate stone attribute text node dumper"),
        clEnumValN(GenStoneAttrNodeTraverse, "gen-stone-attr-node-traverse",
                   "Generate stone attribute traverser"),
        clEnumValN(GenStoneBuiltins, "gen-stone-builtins",
                   "Generate stone builtins list"),
        clEnumValN(GenStoneDiagsDefs, "gen-stone-diags-defs",
                   "Generate Stone diagnostics definitions"),
        clEnumValN(GenStoneDiagGroups, "gen-stone-diag-groups",
                   "Generate Stone diagnostic groups"),
        clEnumValN(GenStoneDiagsIndexName, "gen-stone-diags-index-name",
                   "Generate Stone diagnostic name index"),
        clEnumValN(GenStoneBasicReader, "gen-stone-basic-reader",
                   "Generate Stone BasicReader classes"),
        clEnumValN(GenStoneBasicWriter, "gen-stone-basic-writer",
                   "Generate Stone BasicWriter classes"),
        clEnumValN(GenStoneCommentNodes, "gen-stone-comment-nodes",
                   "Generate Stone AST comment nodes"),
        clEnumValN(GenStoneDeclNodes, "gen-stone-decl-nodes",
                   "Generate Stone AST declaration nodes"),
        clEnumValN(GenStoneStmtNodes, "gen-stone-stmt-nodes",
                   "Generate Stone AST statement nodes"),
        clEnumValN(GenStoneTypeNodes, "gen-stone-type-nodes",
                   "Generate Stone AST type nodes"),
        clEnumValN(GenStoneTypeReader, "gen-stone-type-reader",
                   "Generate Stone AbstractTypeReader class"),
        clEnumValN(GenStoneTypeWriter, "gen-stone-type-writer",
                   "Generate Stone AbstractTypeWriter class"),
        clEnumValN(GenStoneOpcodes, "gen-stone-opcodes",
                   "Generate Stone constexpr interpreter opcodes"),
        clEnumValN(GenStoneSACheckers, "gen-stone-sa-checkers",
                   "Generate Stone Static Analyzer checkers"),
        clEnumValN(GenStoneSyntaxNodeList, "gen-stone-syntax-node-list",
                   "Generate list of Stone Syntax Tree node types"),
        clEnumValN(GenStoneSyntaxNodeClasses, "gen-stone-syntax-node-classes",
                   "Generate definitions of Stone Syntax Tree node clasess"),
        clEnumValN(GenStoneCommentHTMLTags, "gen-stone-comment-html-tags",
                   "Generate efficient matchers for HTML tag "
                   "names that are used in documentation comments"),
        clEnumValN(GenStoneCommentHTMLTagsProperties,
                   "gen-stone-comment-html-tags-properties",
                   "Generate efficient matchers for HTML tag "
                   "properties"),
        clEnumValN(GenStoneCommentHTMLNamedCharacterReferences,
                   "gen-stone-comment-html-named-character-references",
                   "Generate function to translate named character "
                   "references to UTF-8 sequences"),
        clEnumValN(GenStoneCommentCommandInfo, "gen-stone-comment-command-info",
                   "Generate command properties for commands that "
                   "are used in documentation comments"),
        clEnumValN(GenStoneCommentCommandList, "gen-stone-comment-command-list",
                   "Generate list of commands that are used in "
                   "documentation comments"),
        clEnumValN(GenStoneOpenCLBuiltins, "gen-stone-opencl-builtins",
                   "Generate OpenCL builtin declaration handlers"),
        clEnumValN(GenStoneOpenCLBuiltinHeader,
                   "gen-stone-opencl-builtin-header",
                   "Generate OpenCL builtin header"),
        clEnumValN(GenStoneOpenCLBuiltinTests, "gen-stone-opencl-builtin-tests",
                   "Generate OpenCL builtin declaration tests"),
        clEnumValN(GenArmNeon, "gen-arm-neon", "Generate arm_neon.h for stone"),
        clEnumValN(GenArmFP16, "gen-arm-fp16", "Generate arm_fp16.h for stone"),
        clEnumValN(GenArmBF16, "gen-arm-bf16", "Generate arm_bf16.h for stone"),
        clEnumValN(GenArmVectorType, "gen-arm-vector-type",
                   "Generate arm_vector_types.h for stone"),
        clEnumValN(GenArmNeonSema, "gen-arm-neon-sema",
                   "Generate ARM NEON sema support for stone"),
        clEnumValN(GenArmNeonTest, "gen-arm-neon-test",
                   "Generate ARM NEON tests for stone"),
        clEnumValN(GenArmSveHeader, "gen-arm-sve-header",
                   "Generate arm_sve.h for stone"),
        clEnumValN(GenArmSveBuiltins, "gen-arm-sve-builtins",
                   "Generate arm_sve_builtins.inc for stone"),
        clEnumValN(GenArmSveBuiltinCG, "gen-arm-sve-builtin-codegen",
                   "Generate arm_sve_builtin_cg_map.inc for stone"),
        clEnumValN(GenArmSveTypeFlags, "gen-arm-sve-typeflags",
                   "Generate arm_sve_typeflags.inc for stone"),
        clEnumValN(GenArmSveRangeChecks, "gen-arm-sve-sema-rangechecks",
                   "Generate arm_sve_sema_rangechecks.inc for stone"),
        clEnumValN(GenArmSveStreamingAttrs, "gen-arm-sve-streaming-attrs",
                   "Generate arm_sve_streaming_attrs.inc for stone"),
        clEnumValN(GenArmSmeHeader, "gen-arm-sme-header",
                   "Generate arm_sme.h for stone"),
        clEnumValN(GenArmSmeBuiltins, "gen-arm-sme-builtins",
                   "Generate arm_sme_builtins.inc for stone"),
        clEnumValN(GenArmSmeBuiltinCG, "gen-arm-sme-builtin-codegen",
                   "Generate arm_sme_builtin_cg_map.inc for stone"),
        clEnumValN(GenArmSmeRangeChecks, "gen-arm-sme-sema-rangechecks",
                   "Generate arm_sme_sema_rangechecks.inc for stone"),
        clEnumValN(GenArmSmeStreamingAttrs, "gen-arm-sme-streaming-attrs",
                   "Generate arm_sme_streaming_attrs.inc for stone"),
        clEnumValN(GenArmSmeBuiltinZAState, "gen-arm-sme-builtin-za-state",
                   "Generate arm_sme_builtins_za_state.inc for stone"),
        clEnumValN(GenArmMveHeader, "gen-arm-mve-header",
                   "Generate arm_mve.h for stone"),
        clEnumValN(GenArmMveBuiltinDef, "gen-arm-mve-builtin-def",
                   "Generate ARM MVE builtin definitions for stone"),
        clEnumValN(GenArmMveBuiltinSema, "gen-arm-mve-builtin-sema",
                   "Generate ARM MVE builtin sema checks for stone"),
        clEnumValN(GenArmMveBuiltinCG, "gen-arm-mve-builtin-codegen",
                   "Generate ARM MVE builtin code-generator for stone"),
        clEnumValN(GenArmMveBuiltinAliases, "gen-arm-mve-builtin-aliases",
                   "Generate list of valid ARM MVE builtin aliases for stone"),
        clEnumValN(GenArmCdeHeader, "gen-arm-cde-header",
                   "Generate arm_cde.h for stone"),
        clEnumValN(GenArmCdeBuiltinDef, "gen-arm-cde-builtin-def",
                   "Generate ARM CDE builtin definitions for stone"),
        clEnumValN(GenArmCdeBuiltinSema, "gen-arm-cde-builtin-sema",
                   "Generate ARM CDE builtin sema checks for stone"),
        clEnumValN(GenArmCdeBuiltinCG, "gen-arm-cde-builtin-codegen",
                   "Generate ARM CDE builtin code-generator for stone"),
        clEnumValN(GenArmCdeBuiltinAliases, "gen-arm-cde-builtin-aliases",
                   "Generate list of valid ARM CDE builtin aliases for stone"),
        clEnumValN(GenRISCVVectorHeader, "gen-riscv-vector-header",
                   "Generate riscv_vector.h for stone"),
        clEnumValN(GenRISCVVectorBuiltins, "gen-riscv-vector-builtins",
                   "Generate riscv_vector_builtins.inc for stone"),
        clEnumValN(GenRISCVVectorBuiltinCG, "gen-riscv-vector-builtin-codegen",
                   "Generate riscv_vector_builtin_cg.inc for stone"),
        clEnumValN(GenRISCVVectorBuiltinSema, "gen-riscv-vector-builtin-sema",
                   "Generate riscv_vector_builtin_sema.inc for stone"),
        clEnumValN(GenRISCVSiFiveVectorBuiltins,
                   "gen-riscv-sifive-vector-builtins",
                   "Generate riscv_sifive_vector_builtins.inc for stone"),
        clEnumValN(GenRISCVSiFiveVectorBuiltinCG,
                   "gen-riscv-sifive-vector-builtin-codegen",
                   "Generate riscv_sifive_vector_builtin_cg.inc for stone"),
        clEnumValN(GenRISCVSiFiveVectorBuiltinSema,
                   "gen-riscv-sifive-vector-builtin-sema",
                   "Generate riscv_sifive_vector_builtin_sema.inc for stone"),
        clEnumValN(GenAttrDocs, "gen-attr-docs",
                   "Generate attribute documentation"),
        clEnumValN(GenDiagDocs, "gen-diag-docs",
                   "Generate diagnostic documentation"),
        clEnumValN(GenOptDocs, "gen-opt-docs", "Generate option documentation"),
        clEnumValN(GenDataCollectors, "gen-stone-data-collectors",
                   "Generate data collectors for AST nodes"),
        clEnumValN(GenTestPragmaAttributeSupportedAttributes,
                   "gen-stone-test-pragma-attribute-supported-attributes",
                   "Generate a list of attributes supported by #pragma stone "
                   "attribute for testing purposes")));

cl::opt<std::string>
StoneComponent("stone-component",
               cl::desc("Only use warnings from specified component"),
               cl::value_desc("component"), cl::Hidden);

bool StoneTableGenMain(raw_ostream &OS, RecordKeeper &Records) {
  switch (Action) {
  case PrintRecords:
    OS << Records;           // No argument, dump all contents
    break;
  case DumpJSON:
    EmitJSON(Records, OS);
    break;
  case GenStoneAttrClasses:
    EmitStoneAttrClass(Records, OS);
    break;
  case GenStoneAttrParserStringSwitches:
    EmitStoneAttrParserStringSwitches(Records, OS);
    break;
  case GenStoneAttrSubjectMatchRulesParserStringSwitches:
    EmitStoneAttrSubjectMatchRulesParserStringSwitches(Records, OS);
    break;
  case GenStoneAttrImpl:
    EmitStoneAttrImpl(Records, OS);
    break;
  case GenStoneAttrList:
    EmitStoneAttrList(Records, OS);
    break;
  case GenStoneAttrDocTable:
    EmitStoneAttrDocTable(Records, OS);
    break;
  case GenStoneAttrSubjectMatchRuleList:
    EmitStoneAttrSubjectMatchRuleList(Records, OS);
    break;
  case GenStoneAttrPCHRead:
    EmitStoneAttrPCHRead(Records, OS);
    break;
  case GenStoneAttrPCHWrite:
    EmitStoneAttrPCHWrite(Records, OS);
    break;
  case GenStoneRegularKeywordAttributeInfo:
    EmitStoneRegularKeywordAttributeInfo(Records, OS);
    break;
  case GenStoneAttrHasAttributeImpl:
    EmitStoneAttrHasAttrImpl(Records, OS);
    break;
  case GenStoneAttrSpellingListIndex:
    EmitStoneAttrSpellingListIndex(Records, OS);
    break;
  case GenStoneAttrASTVisitor:
    EmitStoneAttrASTVisitor(Records, OS);
    break;
  case GenStoneAttrTemplateInstantiate:
    EmitStoneAttrTemplateInstantiate(Records, OS);
    break;
  case GenStoneAttrParsedAttrList:
    EmitStoneAttrParsedAttrList(Records, OS);
    break;
  case GenStoneAttrParsedAttrImpl:
    EmitStoneAttrParsedAttrImpl(Records, OS);
    break;
  case GenStoneAttrParsedAttrKinds:
    EmitStoneAttrParsedAttrKinds(Records, OS);
    break;
  case GenStoneAttrTextNodeDump:
    EmitStoneAttrTextNodeDump(Records, OS);
    break;
  case GenStoneAttrNodeTraverse:
    EmitStoneAttrNodeTraverse(Records, OS);
    break;
  case GenStoneBuiltins:
    EmitStoneBuiltins(Records, OS);
    break;
  case GenStoneDiagsDefs:
    EmitStoneDiagsDefs(Records, OS, StoneComponent);
    break;
  case GenStoneDiagGroups:
    EmitStoneDiagGroups(Records, OS);
    break;
  case GenStoneDiagsIndexName:
    EmitStoneDiagsIndexName(Records, OS);
    break;
  case GenStoneCommentNodes:
    EmitStoneASTNodes(Records, OS, CommentNodeClassName, "");
    break;
  case GenStoneDeclNodes:
    EmitStoneASTNodes(Records, OS, DeclNodeClassName, "Decl",
                      DeclContextNodeClassName);
    EmitStoneDeclContext(Records, OS);
    break;
  case GenStoneStmtNodes:
    EmitStoneASTNodes(Records, OS, StmtNodeClassName, "");
    break;
  case GenStoneTypeNodes:
    EmitStoneTypeNodes(Records, OS);
    break;
  case GenStoneTypeReader:
    EmitStoneTypeReader(Records, OS);
    break;
  case GenStoneTypeWriter:
    EmitStoneTypeWriter(Records, OS);
    break;
  case GenStoneBasicReader:
    EmitStoneBasicReader(Records, OS);
    break;
  case GenStoneBasicWriter:
    EmitStoneBasicWriter(Records, OS);
    break;
  case GenStoneOpcodes:
    EmitStoneOpcodes(Records, OS);
    break;
  case GenStoneSACheckers:
    EmitStoneSACheckers(Records, OS);
    break;
  case GenStoneCommentHTMLTags:
    EmitStoneCommentHTMLTags(Records, OS);
    break;
  case GenStoneCommentHTMLTagsProperties:
    EmitStoneCommentHTMLTagsProperties(Records, OS);
    break;
  case GenStoneCommentHTMLNamedCharacterReferences:
    EmitStoneCommentHTMLNamedCharacterReferences(Records, OS);
    break;
  case GenStoneCommentCommandInfo:
    EmitStoneCommentCommandInfo(Records, OS);
    break;
  case GenStoneCommentCommandList:
    EmitStoneCommentCommandList(Records, OS);
    break;
  case GenStoneOpenCLBuiltins:
    EmitStoneOpenCLBuiltins(Records, OS);
    break;
  case GenStoneOpenCLBuiltinHeader:
    EmitStoneOpenCLBuiltinHeader(Records, OS);
    break;
  case GenStoneOpenCLBuiltinTests:
    EmitStoneOpenCLBuiltinTests(Records, OS);
    break;
  case GenStoneSyntaxNodeList:
    EmitStoneSyntaxNodeList(Records, OS);
    break;
  case GenStoneSyntaxNodeClasses:
    EmitStoneSyntaxNodeClasses(Records, OS);
    break;
  case GenArmNeon:
    EmitNeon(Records, OS);
    break;
  case GenArmFP16:
    EmitFP16(Records, OS);
    break;
  case GenArmVectorType:
    EmitVectorTypes(Records, OS);
    break;
  case GenArmBF16:
    EmitBF16(Records, OS);
    break;
  case GenArmNeonSema:
    EmitNeonSema(Records, OS);
    break;
  case GenArmNeonTest:
    EmitNeonTest(Records, OS);
    break;
  case GenArmMveHeader:
    EmitMveHeader(Records, OS);
    break;
  case GenArmMveBuiltinDef:
    EmitMveBuiltinDef(Records, OS);
    break;
  case GenArmMveBuiltinSema:
    EmitMveBuiltinSema(Records, OS);
    break;
  case GenArmMveBuiltinCG:
    EmitMveBuiltinCG(Records, OS);
    break;
  case GenArmMveBuiltinAliases:
    EmitMveBuiltinAliases(Records, OS);
    break;
  case GenArmSveHeader:
    EmitSveHeader(Records, OS);
    break;
  case GenArmSveBuiltins:
    EmitSveBuiltins(Records, OS);
    break;
  case GenArmSveBuiltinCG:
    EmitSveBuiltinCG(Records, OS);
    break;
  case GenArmSveTypeFlags:
    EmitSveTypeFlags(Records, OS);
    break;
  case GenArmSveRangeChecks:
    EmitSveRangeChecks(Records, OS);
    break;
  case GenArmSveStreamingAttrs:
    EmitSveStreamingAttrs(Records, OS);
    break;
  case GenArmSmeHeader:
    EmitSmeHeader(Records, OS);
    break;
  case GenArmSmeBuiltins:
    EmitSmeBuiltins(Records, OS);
    break;
  case GenArmSmeBuiltinCG:
    EmitSmeBuiltinCG(Records, OS);
    break;
  case GenArmSmeRangeChecks:
    EmitSmeRangeChecks(Records, OS);
    break;
  case GenArmSmeStreamingAttrs:
    EmitSmeStreamingAttrs(Records, OS);
    break;
  case GenArmSmeBuiltinZAState:
    EmitSmeBuiltinZAState(Records, OS);
    break;
  case GenArmCdeHeader:
    EmitCdeHeader(Records, OS);
    break;
  case GenArmCdeBuiltinDef:
    EmitCdeBuiltinDef(Records, OS);
    break;
  case GenArmCdeBuiltinSema:
    EmitCdeBuiltinSema(Records, OS);
    break;
  case GenArmCdeBuiltinCG:
    EmitCdeBuiltinCG(Records, OS);
    break;
  case GenArmCdeBuiltinAliases:
    EmitCdeBuiltinAliases(Records, OS);
    break;
  case GenRISCVVectorHeader:
    EmitRVVHeader(Records, OS);
    break;
  case GenRISCVVectorBuiltins:
    EmitRVVBuiltins(Records, OS);
    break;
  case GenRISCVVectorBuiltinCG:
    EmitRVVBuiltinCG(Records, OS);
    break;
  case GenRISCVVectorBuiltinSema:
    EmitRVVBuiltinSema(Records, OS);
    break;
  case GenRISCVSiFiveVectorBuiltins:
    EmitRVVBuiltins(Records, OS);
    break;
  case GenRISCVSiFiveVectorBuiltinCG:
    EmitRVVBuiltinCG(Records, OS);
    break;
  case GenRISCVSiFiveVectorBuiltinSema:
    EmitRVVBuiltinSema(Records, OS);
    break;
  case GenAttrDocs:
    EmitStoneAttrDocs(Records, OS);
    break;
  case GenDiagDocs:
    EmitStoneDiagDocs(Records, OS);
    break;
  case GenOptDocs:
    EmitStoneOptDocs(Records, OS);
    break;
  case GenDataCollectors:
    EmitStoneDataCollectors(Records, OS);
    break;
  case GenTestPragmaAttributeSupportedAttributes:
    EmitTestPragmaAttributeSupportedAttributes(Records, OS);
    break;
  }

  return false;
}
}

int main(int argc, char **argv) {
  sys::PrintStackTraceOnErrorSignal(argv[0]);
  PrettyStackTraceProgram X(argc, argv);
  cl::ParseCommandLineOptions(argc, argv);

  llvm_shutdown_obj Y;

  return TableGenMain(argv[0], &StoneTableGenMain);
}

#ifdef __has_feature
#if __has_feature(address_sanitizer)
#include <sanitizer/lsan_interface.h>
// Disable LeakSanitizer for this binary as it has too many leaks that are not
// very interesting to fix. See compiler-rt/include/sanitizer/lsan_interface.h .
int __lsan_is_turned_off() { return 1; }
#endif  // __has_feature(address_sanitizer)
#endif  // defined(__has_feature)
