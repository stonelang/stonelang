#ifndef STONE_AST_MEMORYMANAGER_H
#define STONE_AST_MEMORYMANAGER_H

#include "llvm/ADT/ArrayRef.h"
#include "llvm/ADT/SetVector.h"
#include "llvm/ADT/SmallVector.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/Support/Allocator.h"

namespace stone {
class MemoryManager final {
  mutable llvm::BumpPtrAllocator allocator;

public:
  MemoryManager() = default;
  MemoryManager(const MemoryManager &) = delete;
  MemoryManager &operator=(const MemoryManager &) = delete;

public:
  /// Raw memory allocation
  void *AllocateMemory(size_t bytes, unsigned alignment = 8) const {
    return bytes ? allocator.Allocate(bytes, alignment) : nullptr;
  }

  template <typename T> T *AllocateMemory(size_t count = 1) const {
    return static_cast<T *>(AllocateMemory(sizeof(T) * count, alignof(T)));
  }

  /// Allocate and construct a single object
  template <typename T> T *Allocate() const {
    T *res = AllocateMemory<T>();
    new (res) T();
    return res;
  }

  /// Allocate and construct N default-initialized objects
  template <typename T>
  llvm::MutableArrayRef<T> Allocate(size_t numElts) const {
    T *data = AllocateMemory<T>(numElts);
    for (size_t i = 0; i != numElts; ++i)
      new (data + i) T();
    return {data, numElts};
  }

  /// Allocate without construction
  template <typename T>
  llvm::MutableArrayRef<T> AllocateUninitialized(size_t numElts) const {
    return {AllocateMemory<T>(numElts), numElts};
  }

  /// Allocate and copy an object
  template <typename T> T *AllocateCopy(T &&val) const {
    using TNoRef = typename std::remove_reference<T>::type;
    TNoRef *res = AllocateMemory<TNoRef>();
    new (res) TNoRef(std::forward<T>(val));
    return res;
  }

  /// Allocate and copy from iterator range
  template <typename T, typename It> T *AllocateCopy(It start, It end) const {
    size_t count = static_cast<size_t>(std::distance(start, end));
    T *data = AllocateMemory<T>(count);
    for (size_t i = 0; i < count; ++i, ++start)
      new (data + i) T(*start);
    return data;
  }

  /// Allocate and copy from ArrayRef
  template <typename T>
  llvm::MutableArrayRef<T> AllocateCopy(llvm::ArrayRef<T> array) const {
    return {AllocateCopy<T>(array.begin(), array.end()), array.size()};
  }

  /// Allocate and copy from SmallVector
  template <typename T>
  llvm::ArrayRef<T> AllocateCopy(const llvm::SmallVectorImpl<T> &vec) const {
    return AllocateCopy(llvm::ArrayRef<T>(vec));
  }

  template <typename T>
  llvm::MutableArrayRef<T> AllocateCopy(llvm::SmallVectorImpl<T> &vec) const {
    return AllocateCopy(llvm::MutableArrayRef<T>(vec));
  }

  /// Allocate and copy a string
  llvm::StringRef AllocateCopy(llvm::StringRef str) const {
    auto data = AllocateCopy<char>(str.begin(), str.end());
    return llvm::StringRef(data, str.size());
  }

  /// Allocate and copy from SetVector
  template <typename T, typename Vector, typename Set>
  llvm::MutableArrayRef<T>
  AllocateCopy(llvm::SetVector<T, Vector, Set> setVec) const {
    return {AllocateCopy<T>(setVec.begin(), setVec.end()), setVec.size()};
  }

  template <typename T, typename... Args> T *Create(Args &&...args) const {
    T *res = AllocateMemory<T>();
    new (res) T(std::forward<Args>(args)...);
    return res;
  }

  /// Create and initialize an array of objects
  template <typename T, typename... Args>
  llvm::MutableArrayRef<T> CreateArray(size_t count, Args &&...args) const {
    T *data = AllocateMemory<T>(count);
    for (size_t i = 0; i < count; ++i)
      new (data + i) T(std::forward<Args>(args)...);
    return {data, count};
  }

  /// Access allocator
  llvm::BumpPtrAllocator &GetAllocator() const { return allocator; }
  /// Get total memory used
  size_t GetTotalMemoryAllocated() const { return allocator.getTotalMemory(); }
};

} // namespace stone
#endif // STONE_AST_MEMORY_H
