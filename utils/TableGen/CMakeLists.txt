set(LLVM_LINK_COMPONENTS Support)

add_tablegen(stone-tblgen STONE
  DESTINATION "${STONE_TOOLS_INSTALL_DIR}"
  EXPORT Stone
  ASTTableGen.cpp
  ClangASTNodesEmitter.cpp
  ClangASTPropertiesEmitter.cpp
  ClangAttrEmitter.cpp
  ClangBuiltinsEmitter.cpp
  ClangCommentCommandInfoEmitter.cpp
  ClangCommentHTMLNamedCharacterReferenceEmitter.cpp
  ClangCommentHTMLTagsEmitter.cpp
  ClangDataCollectorsEmitter.cpp
  ClangDiagnosticsEmitter.cpp
  ClangOpcodesEmitter.cpp
  ClangOpenCLBuiltinEmitter.cpp
  ClangOptionDocEmitter.cpp
  ClangSACheckersEmitter.cpp
  ClangSyntaxEmitter.cpp
  ClangTypeNodesEmitter.cpp
  MveEmitter.cpp
  NeonEmitter.cpp
  RISCVVEmitter.cpp
  SveEmitter.cpp
  TableGen.cpp
  )

target_link_libraries(stone-tblgen PRIVATE stoneSupport_tablegen)
