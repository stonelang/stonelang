#ifndef STONE_AST_ASTSESSION_H
#define STONE_AST_ASTSESSION_H

#include "stone/AST/Foreign.h"
#include "stone/AST/Identifier.h"
#include "stone/AST/SourceFileKind.h"

#include "llvm/ADT/ArrayRef.h"
#include "llvm/ADT/SetVector.h"
#include "llvm/ADT/SmallVector.h"
#include "llvm/ADT/StringMap.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/Support/Allocator.h"
#include "llvm/Support/Casting.h"

#include <memory>

namespace stone {
class Type;
class FunDecl;
class Scaffolder;
class TypeChecker;
class JoinDecl;
class SpaceDecl;
class UsingDecl;
class SourceFile;
class ModuleDecl;
class NormalModuleDecl;
class BuiltinModuleDecl;
class ForeignModuleDecl;

class ASTMemory {
protected:
  mutable llvm::BumpPtrAllocator allocator;

public:
  ASTMemory() = default;
  ASTMemory(const ASTMemory &) = delete;
  ASTMemory &operator=(const ASTMemory &) = delete;

  void *AllocateMemory(size_t bytes, unsigned alignment = 8) const {
    return (bytes == 0) ? nullptr : allocator.Allocate(bytes, alignment);
  }

  template <typename T> T *AllocateMemory(size_t count = 1) const {
    return static_cast<T *>(AllocateMemory(sizeof(T) * count, alignof(T)));
  }

  template <typename T> T *Allocate() const {
    auto *res = AllocateMemory<T>();
    new (res) T();
    return res;
  }

  // ...all your AllocateCopy and ArrayRef methods move here ...

  llvm::BumpPtrAllocator &GetAllocator() const { return allocator; }
  size_t GetTotalMemoryAllocated() const { return allocator.getTotalMemory(); }
};

class ASTSession final : public ASTMemory {
  friend Scaffolder;
  friend TypeChecker;

  using IdentifierTable = llvm::StringMap<char, llvm::BumpPtrAllocator &>;
  mutable IdentifierTable identifierTable;

public:
  ASTSession(const ASTSession &) = delete;
  ASTSession &operator=(const ASTSession &) = delete;

  ASTSession();
  ~ASTSession();

public:
  // Declare the set of builtin identifiers.
#define BUILTIN_TYPE(ID, Parent) const Type *Builtin##ID##Type;
#include "stone/AST/TypeNode.def"

  // Declare the set of builtin identifiers.
#define BUILTIN_IDENTIFIER_WITH_NAME(Name, IdStr)                              \
  Identifier Builtin##Name##Identifier;
#include "stone/AST/BuiltinIdentifiers.def"
  Identifier GetIdentifier(llvm::StringRef text) const;

public:
  //   /// Allocate memory from the ASTSession bump pointer.
  //   void *AllocateMemory(size_t bytes, unsigned alignment = 8) const {
  //     if (bytes == 0) {
  //       return nullptr;
  //     }
  //     return GetAllocator().Allocate(bytes, alignment);
  //   }

  //   template <typename T> T *AllocateMemory(size_t num = 1) const {
  //     return static_cast<T *>(AllocateMemory(num * sizeof(T), alignof(T)));
  //   }

  //   /// Memory allocator
  //   llvm::BumpPtrAllocator &GetAllocator() const { return allocator; }

  //   /// The total amount of memory used
  //   size_t GetTotalMemoryAllocated() const {
  //     return GetAllocator().getTotalMemory();
  //   }

  // public:
  //   template <typename T> T *Allocate() const {
  //     T *res = (T *)AllocateMemory(sizeof(T), alignof(T));
  //     new (res) T();
  //     return res;
  //   }

  //   template <typename T>
  //   llvm::MutableArrayRef<T> AllocateUninitialized(unsigned NumElts) const {
  //     T *Data = (T *)AllocateMemory(sizeof(T) * NumElts, alignof(T));
  //     return {Data, NumElts};
  //   }

  //   template <typename T>
  //   llvm::MutableArrayRef<T> Allocate(unsigned numElts) const {
  //     T *res = (T *)AllocateMemory(sizeof(T) * numElts, alignof(T));
  //     for (unsigned i = 0; i != numElts; ++i)
  //       new (res + i) T();
  //     return {res, numElts};
  //   }

  //   /// Allocate a copy of the specified object.
  //   template <typename T>
  //   typename std::remove_reference<T>::type *AllocateObjectCopy(T &&t) const
  //   {
  //     // This function cannot be named AllocateCopy because it would always
  //     win
  //     // overload resolution over the AllocateCopy(ArrayRef<T>).
  //     using TNoRef = typename std::remove_reference<T>::type;
  //     TNoRef *res = (TNoRef *)AllocateMemory(sizeof(TNoRef),
  //     alignof(TNoRef)); new (res) TNoRef(std::forward<T>(t)); return res;
  //   }

  //   template <typename T, typename It> T *AllocateCopy(It start, It end)
  //   const {
  //     T *res = (T *)AllocateMemory(sizeof(T) * (end - start), alignof(T));
  //     for (unsigned i = 0; start != end; ++start, ++i)
  //       new (res + i) T(*start);
  //     return res;
  //   }

  //   template <typename T, size_t N>
  //   llvm::MutableArrayRef<T> AllocateCopy(T (&array)[N]) const {
  //     return llvm::MutableArrayRef<T>(AllocateCopy<T>(array, array + N), N);
  //   }

  //   template <typename T>
  //   llvm::MutableArrayRef<T> AllocateCopy(llvm::ArrayRef<T> array) const {
  //     return llvm::MutableArrayRef<T>(AllocateCopy<T>(array.begin(),
  //     array.end()),
  //                                     array.size());
  //   }

  //   template <typename T>
  //   llvm::ArrayRef<T> AllocateCopy(const llvm::SmallVectorImpl<T> &vec) const
  //   {
  //     return AllocateCopy(ArrayRef<T>(vec));
  //   }

  //   template <typename T>
  //   llvm::MutableArrayRef<T> AllocateCopy(llvm::SmallVectorImpl<T> &vec)
  //   const {
  //     return AllocateCopy(llvm::MutableArrayRef<T>(vec));
  //   }

  //   llvm::StringRef AllocateCopy(llvm::StringRef Str) const {
  //     llvm::ArrayRef<char> Result =
  //         AllocateCopy(llvm::ArrayRef(Str.data(), Str.size()));
  //     return llvm::StringRef(Result.data(), Result.size());
  //   }

  //   template <typename T, typename Vector, typename Set>
  //   llvm::MutableArrayRef<T>
  //   AllocateCopy(llvm::SetVector<T, Vector, Set> setVector) const {
  //     return llvm::MutableArrayRef<T>(
  //         AllocateCopy<T>(setVector.begin(), setVector.end()),
  //         setVector.size());
  //   }

public:
  SourceFile *CreateSourceFile(SourceFileKind kind, unsigned srcBufferID,
                               ModuleDecl *owner);

public:
  FunDecl *CreateFunDecl();
  JoinDecl *CreateJoinDecl();
  SpaceDecl *CreateSpaceDecl();
  UsingDecl *CreateUsingDecl();
  NormalModuleDecl *CreateNormalModuleDecl();
  BuiltinModuleDecl *CreateBuiltinModuleDecl();
  ForeignModuleDecl *CreateForeignModuleDecl(ForeignModuleDeclKind kind);
};

} // namespace stone

#endif