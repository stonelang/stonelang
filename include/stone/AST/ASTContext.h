#ifndef STONE_AST_ASTCONTEXT_H
#define STONE_AST_ASTCONTEXT_H

#include "stone/AST/Identifier.h"

#include "llvm/ADT/ArrayRef.h"
#include "llvm/ADT/SetVector.h"
#include "llvm/ADT/SmallVector.h"
#include "llvm/ADT/StringMap.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/Support/Allocator.h"
#include "llvm/Support/Casting.h"

namespace stone {
class Type;

class ASTContext final {

  mutable llvm::BumpPtrAllocator allocator;
  using IdentifierTable = llvm::StringMap<char, llvm::BumpPtrAllocator &>;
  mutable IdentifierTable identifierTable;

public:
  const Type *BuiltinFloat16Type;  /// 32-bit IEEE floating point
  const Type *BuiltinFloat32Type;  /// 32-bit IEEE floating point
  const Type *BuiltinFloat64Type;  /// 64-bit IEEE floating point
  const Type *BuiltinFloat128Type; /// 128-bit IEEE floating point
  const Type *BuiltinFloatType;    /// 128-bit IEEE floating point

  const Type *BuiltinInt8Type;
  const Type *BuiltinInt16Type;
  const Type *BuiltinInt32Type;
  const Type *BuiltinInt64Type;
  const Type *BuiltinInt128Type;
  const Type *BuiltinIntType;

  const Type *BuiltinUInt8Type;
  const Type *BuiltinUInt16Type;
  const Type *BuiltinUInt32Type;
  const Type *BuiltinUInt64Type;
  const Type *BuiltinUInt128Type;
  const Type *BuiltinUIntType;

  const Type *BuiltinAutoType;
  const Type *BuiltinVoidType;
  const Type *BuiltinNullType;
  const Type *BuiltinBoolType;

public:
  ASTContext(const ASTContext &) = delete;
  ASTContext &operator=(const ASTContext &) = delete;

  ASTContext();
  ~ASTContext();

public:
  // Declare the set of builtin identifiers.
#define BUILTIN_IDENTIFIER_WITH_NAME(Name, IdStr)                              \
  Identifier Builtin##Name##Identifier;
#include "stone/AST/BuiltinIdentifiers.def"
  Identifier GetIdentifier(llvm::StringRef text) const;

public:
public:
  /// Allocate memory from the ASTContext bump pointer.
  void *AllocateMemory(size_t bytes, unsigned alignment = 8) const {
    if (bytes == 0) {
      return nullptr;
    }
    return GetAllocator().Allocate(bytes, alignment);
  }

  template <typename T> T *AllocateMemory(size_t num = 1) const {
    return static_cast<T *>(AllocateMemory(num * sizeof(T), alignof(T)));
  }

  /// Deallocate
  void Deallocate(void *Ptr) const {}

  /// Memory allocator
  llvm::BumpPtrAllocator &GetAllocator() const { return allocator; }

  /// The total amount of memory used
  size_t GetTotalMemoryAllocated() const {
    return GetAllocator().getTotalMemory();
  }

public:
  template <typename T> T *Allocate() const {
    T *res = (T *)AllocateMemory(sizeof(T), alignof(T));
    new (res) T();
    return res;
  }

  template <typename T>
  llvm::MutableArrayRef<T> AllocateUninitialized(unsigned NumElts) const {
    T *Data = (T *)AllocateMemory(sizeof(T) * NumElts, alignof(T));
    return {Data, NumElts};
  }

  template <typename T>
  llvm::MutableArrayRef<T> Allocate(unsigned numElts) const {
    T *res = (T *)AllocateMemory(sizeof(T) * numElts, alignof(T));
    for (unsigned i = 0; i != numElts; ++i)
      new (res + i) T();
    return {res, numElts};
  }

  /// Allocate a copy of the specified object.
  template <typename T>
  typename std::remove_reference<T>::type *AllocateObjectCopy(T &&t) const {
    // This function cannot be named AllocateCopy because it would always win
    // overload resolution over the AllocateCopy(ArrayRef<T>).
    using TNoRef = typename std::remove_reference<T>::type;
    TNoRef *res = (TNoRef *)AllocateMemory(sizeof(TNoRef), alignof(TNoRef));
    new (res) TNoRef(std::forward<T>(t));
    return res;
  }

  template <typename T, typename It> T *AllocateCopy(It start, It end) const {
    T *res = (T *)AllocateMemory(sizeof(T) * (end - start), alignof(T));
    for (unsigned i = 0; start != end; ++start, ++i)
      new (res + i) T(*start);
    return res;
  }

  template <typename T, size_t N>
  llvm::MutableArrayRef<T> AllocateCopy(T (&array)[N]) const {
    return llvm::MutableArrayRef<T>(AllocateCopy<T>(array, array + N), N);
  }

  template <typename T>
  llvm::MutableArrayRef<T> AllocateCopy(llvm::ArrayRef<T> array) const {
    return llvm::MutableArrayRef<T>(AllocateCopy<T>(array.begin(), array.end()),
                                    array.size());
  }

  template <typename T>
  llvm::ArrayRef<T> AllocateCopy(const llvm::SmallVectorImpl<T> &vec) const {
    return AllocateCopy(ArrayRef<T>(vec));
  }

  template <typename T>
  llvm::MutableArrayRef<T> AllocateCopy(llvm::SmallVectorImpl<T> &vec) const {
    return AllocateCopy(llvm::MutableArrayRef<T>(vec));
  }

  llvm::StringRef AllocateCopy(llvm::StringRef Str) const {
    llvm::ArrayRef<char> Result =
        AllocateCopy(llvm::ArrayRef(Str.data(), Str.size()));
    return llvm::StringRef(Result.data(), Result.size());
  }

  template <typename T, typename Vector, typename Set>
  llvm::MutableArrayRef<T>
  AllocateCopy(llvm::SetVector<T, Vector, Set> setVector) const {
    return llvm::MutableArrayRef<T>(
        AllocateCopy<T>(setVector.begin(), setVector.end()), setVector.size());
  }
};

} // namespace stone

#endif