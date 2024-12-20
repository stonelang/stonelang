#ifndef STONE_COMPILE_COMPILE_H
#define STONE_COMPILE_COMPILE_H

#include "stone/Basic/CodeGenOptions.h"
#include "stone/CodeGen/CodeGenModule.h"
#include "stone/Frontend/FrontendOptions.h"

#include "llvm/ADT/ArrayRef.h"
#include "llvm/IR/Module.h"

namespace stone {
class SourceFile;
class LangOptions;
class ClangImporter;
class FrontendInstance;
class PrimaryFileSpecificPaths;
class FrontendObservation;
class CodeGenResult;

int Compile(llvm::ArrayRef<const char *> args, const char *arg0, void *mainAddr,
            FrontendObservation *observation = nullptr);

/// \return true if compilie is successful
bool PerformCompile(FrontendInstance &instance);

/// \retyrb true if we compiled an ir file.
bool PerformCompileLLVM(FrontendInstance &compiler);

/// \return true if syntax analysis is successful
bool PerformParse(FrontendInstance &instance);

/// \return true if syntax analysis is successful for a specific source file
bool PerformParse(FrontendInstance &instance, SourceFile &sourceFile);

/// \return true if syntax analysis is successful for a specific source file
bool PerformEmitParse(FrontendInstance &instance);

// \return true if syntax analysis is successful
bool PerformResolveImports(FrontendInstance &instance);

// \resolve all of the source files for this import
bool PerformResolveImports(FrontendInstance &instance, SourceFile &sourceFile);

// \return true if semantic analysis is successful
bool PerformSemanticAnalysis(FrontendInstance &instance);

// \return true if semantic analysis is successful
bool PerformSemanticAnalysis(FrontendInstance &instance,
                             SourceFile &sourceFile);

// \return true if emit-ast is true
bool PerformEmitAST(FrontendInstance &instance);

// \return true if the code generation was successfull
bool CompletedSemanticAnalysis(FrontendInstance &instance);

// // \return llvm::Module if IR generation is successful
// CodeGenResult PerformCodeGenIR(FrontendInstance &instance,
//                                SourceFile *sourceFile,
//                                llvm::StringRef moduleName,
//                                const PrimaryFileSpecificPaths &sps,
//                                llvm::GlobalVariable *&globalHash);

// ///\return the generated module
// CodeGenResult PerformCodeGenIR(FrontendInstance &instance,
//                                ModuleDecl *moduleDecl,
//                                llvm::StringRef moduleName,
//                                const PrimaryFileSpecificPaths &sps,
//                                ArrayRef<std::string> parallelOutputFilenames,
//                                llvm::GlobalVariable *&globalHash);

// // \return true if syntax analysis is successful
// bool PerformCodeGenBackend(FrontendInstance &instance,
//                            llvm::StringRef outputFilename, llvm::Module
//                            *module, llvm::GlobalVariable *&globalHash);

// ///\return the IRTargetOptions
// IRTargetOptions GetIRTargetOptions(const CodeGenOptions &codeGenOpts,
//                                    const LangOptions &langOpts,
//                                    ClangImporter &clangImporter);
} // namespace stone

#endif