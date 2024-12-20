//===- TableGenBackends.h - Declarations for Stone TableGen Backends ------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file contains the declarations for all of the Stone TableGen
// backends. A "TableGen backend" is just a function. See
// "$LLVM_ROOT/utils/TableGen/TableGenBackends.h" for more info.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_STONE_UTILS_TABLEGEN_TABLEGENBACKENDS_H
#define LLVM_STONE_UTILS_TABLEGEN_TABLEGENBACKENDS_H

#include <string>

namespace llvm {
class raw_ostream;
class RecordKeeper;
} // namespace llvm

namespace stone {

void EmitStoneDeclContext(llvm::RecordKeeper &RK, llvm::raw_ostream &OS);
/**
  @param PriorizeIfSubclassOf These classes should be prioritized in the output.
  This is useful to force enum generation/jump tables/lookup tables to be more
  compact in both size and surrounding code in hot functions. An example use is
  in Decl for classes that inherit from DeclContext, for functions like
  castFromDeclContext.
  */
void EmitStoneASTNodes(llvm::RecordKeeper &RK, llvm::raw_ostream &OS,
                       const std::string &N, const std::string &S,
                       std::string_view PriorizeIfSubclassOf = "");
void EmitStoneBasicReader(llvm::RecordKeeper &Records, llvm::raw_ostream &OS);
void EmitStoneBasicWriter(llvm::RecordKeeper &Records, llvm::raw_ostream &OS);
void EmitStoneTypeNodes(llvm::RecordKeeper &Records, llvm::raw_ostream &OS);
void EmitStoneTypeReader(llvm::RecordKeeper &Records, llvm::raw_ostream &OS);
void EmitStoneTypeWriter(llvm::RecordKeeper &Records, llvm::raw_ostream &OS);
void EmitStoneAttrParserStringSwitches(llvm::RecordKeeper &Records,
                                       llvm::raw_ostream &OS);
void EmitStoneAttrSubjectMatchRulesParserStringSwitches(
    llvm::RecordKeeper &Records, llvm::raw_ostream &OS);
void EmitStoneAttrClass(llvm::RecordKeeper &Records, llvm::raw_ostream &OS);
void EmitStoneAttrImpl(llvm::RecordKeeper &Records, llvm::raw_ostream &OS);
void EmitStoneAttrList(llvm::RecordKeeper &Records, llvm::raw_ostream &OS);
void EmitStoneAttrSubjectMatchRuleList(llvm::RecordKeeper &Records,
                                       llvm::raw_ostream &OS);
void EmitStoneAttrPCHRead(llvm::RecordKeeper &Records, llvm::raw_ostream &OS);
void EmitStoneAttrPCHWrite(llvm::RecordKeeper &Records, llvm::raw_ostream &OS);
void EmitStoneRegularKeywordAttributeInfo(llvm::RecordKeeper &Records,
                                          llvm::raw_ostream &OS);
void EmitStoneAttrHasAttrImpl(llvm::RecordKeeper &Records,
                              llvm::raw_ostream &OS);
void EmitStoneAttrSpellingListIndex(llvm::RecordKeeper &Records,
                                    llvm::raw_ostream &OS);
void EmitStoneAttrASTVisitor(llvm::RecordKeeper &Records,
                             llvm::raw_ostream &OS);
void EmitStoneAttrTemplateInstantiate(llvm::RecordKeeper &Records,
                                      llvm::raw_ostream &OS);
void EmitStoneAttrParsedAttrList(llvm::RecordKeeper &Records,
                                 llvm::raw_ostream &OS);
void EmitStoneAttrParsedAttrImpl(llvm::RecordKeeper &Records,
                                 llvm::raw_ostream &OS);
void EmitStoneAttrParsedAttrKinds(llvm::RecordKeeper &Records,
                                  llvm::raw_ostream &OS);
void EmitStoneAttrTextNodeDump(llvm::RecordKeeper &Records,
                               llvm::raw_ostream &OS);
void EmitStoneAttrNodeTraverse(llvm::RecordKeeper &Records,
                               llvm::raw_ostream &OS);
void EmitStoneAttrDocTable(llvm::RecordKeeper &Records, llvm::raw_ostream &OS);

void EmitStoneBuiltins(llvm::RecordKeeper &Records, llvm::raw_ostream &OS);

void EmitStoneDiagsDefs(llvm::RecordKeeper &Records, llvm::raw_ostream &OS,
                        const std::string &Component);
void EmitStoneDiagGroups(llvm::RecordKeeper &Records, llvm::raw_ostream &OS);
void EmitStoneDiagsIndexName(llvm::RecordKeeper &Records,
                             llvm::raw_ostream &OS);

void EmitStoneSACheckers(llvm::RecordKeeper &Records, llvm::raw_ostream &OS);

void EmitStoneCommentHTMLTags(llvm::RecordKeeper &Records,
                              llvm::raw_ostream &OS);
void EmitStoneCommentHTMLTagsProperties(llvm::RecordKeeper &Records,
                                        llvm::raw_ostream &OS);
void EmitStoneCommentHTMLNamedCharacterReferences(llvm::RecordKeeper &Records,
                                                  llvm::raw_ostream &OS);

void EmitStoneCommentCommandInfo(llvm::RecordKeeper &Records,
                                 llvm::raw_ostream &OS);
void EmitStoneCommentCommandList(llvm::RecordKeeper &Records,
                                 llvm::raw_ostream &OS);
void EmitStoneOpcodes(llvm::RecordKeeper &Records, llvm::raw_ostream &OS);

void EmitStoneSyntaxNodeList(llvm::RecordKeeper &Records,
                             llvm::raw_ostream &OS);
void EmitStoneSyntaxNodeClasses(llvm::RecordKeeper &Records,
                                llvm::raw_ostream &OS);

void EmitNeon(llvm::RecordKeeper &Records, llvm::raw_ostream &OS);
void EmitFP16(llvm::RecordKeeper &Records, llvm::raw_ostream &OS);
void EmitBF16(llvm::RecordKeeper &Records, llvm::raw_ostream &OS);
void EmitNeonSema(llvm::RecordKeeper &Records, llvm::raw_ostream &OS);
void EmitVectorTypes(llvm::RecordKeeper &Records, llvm::raw_ostream &OS);
void EmitNeonTest(llvm::RecordKeeper &Records, llvm::raw_ostream &OS);

void EmitSveHeader(llvm::RecordKeeper &Records, llvm::raw_ostream &OS);
void EmitSveBuiltins(llvm::RecordKeeper &Records, llvm::raw_ostream &OS);
void EmitSveBuiltinCG(llvm::RecordKeeper &Records, llvm::raw_ostream &OS);
void EmitSveTypeFlags(llvm::RecordKeeper &Records, llvm::raw_ostream &OS);
void EmitSveRangeChecks(llvm::RecordKeeper &Records, llvm::raw_ostream &OS);
void EmitSveStreamingAttrs(llvm::RecordKeeper &Records, llvm::raw_ostream &OS);

void EmitSmeHeader(llvm::RecordKeeper &Records, llvm::raw_ostream &OS);
void EmitSmeBuiltins(llvm::RecordKeeper &Records, llvm::raw_ostream &OS);
void EmitSmeBuiltinCG(llvm::RecordKeeper &Records, llvm::raw_ostream &OS);
void EmitSmeRangeChecks(llvm::RecordKeeper &Records, llvm::raw_ostream &OS);
void EmitSmeStreamingAttrs(llvm::RecordKeeper &Records, llvm::raw_ostream &OS);
void EmitSmeBuiltinZAState(llvm::RecordKeeper &Records, llvm::raw_ostream &OS);

void EmitMveHeader(llvm::RecordKeeper &Records, llvm::raw_ostream &OS);
void EmitMveBuiltinDef(llvm::RecordKeeper &Records, llvm::raw_ostream &OS);
void EmitMveBuiltinSema(llvm::RecordKeeper &Records, llvm::raw_ostream &OS);
void EmitMveBuiltinCG(llvm::RecordKeeper &Records, llvm::raw_ostream &OS);
void EmitMveBuiltinAliases(llvm::RecordKeeper &Records, llvm::raw_ostream &OS);

void EmitRVVHeader(llvm::RecordKeeper &Records, llvm::raw_ostream &OS);
void EmitRVVBuiltins(llvm::RecordKeeper &Records, llvm::raw_ostream &OS);
void EmitRVVBuiltinCG(llvm::RecordKeeper &Records, llvm::raw_ostream &OS);
void EmitRVVBuiltinSema(llvm::RecordKeeper &Records, llvm::raw_ostream &OS);

void EmitCdeHeader(llvm::RecordKeeper &Records, llvm::raw_ostream &OS);
void EmitCdeBuiltinDef(llvm::RecordKeeper &Records, llvm::raw_ostream &OS);
void EmitCdeBuiltinSema(llvm::RecordKeeper &Records, llvm::raw_ostream &OS);
void EmitCdeBuiltinCG(llvm::RecordKeeper &Records, llvm::raw_ostream &OS);
void EmitCdeBuiltinAliases(llvm::RecordKeeper &Records, llvm::raw_ostream &OS);

void EmitStoneAttrDocs(llvm::RecordKeeper &Records, llvm::raw_ostream &OS);
void EmitStoneDiagDocs(llvm::RecordKeeper &Records, llvm::raw_ostream &OS);
void EmitStoneOptDocs(llvm::RecordKeeper &Records, llvm::raw_ostream &OS);

void EmitStoneOpenCLBuiltins(llvm::RecordKeeper &Records,
                             llvm::raw_ostream &OS);
void EmitStoneOpenCLBuiltinHeader(llvm::RecordKeeper &Records,
                                  llvm::raw_ostream &OS);
void EmitStoneOpenCLBuiltinTests(llvm::RecordKeeper &Records,
                                 llvm::raw_ostream &OS);

void EmitStoneDataCollectors(llvm::RecordKeeper &Records,
                             llvm::raw_ostream &OS);

void EmitTestPragmaAttributeSupportedAttributes(llvm::RecordKeeper &Records,
                                                llvm::raw_ostream &OS);

} // end namespace stone

#endif
