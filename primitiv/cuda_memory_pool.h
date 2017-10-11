#ifndef PRIMITIV_CUDA_MEMORY_POOL_H_
#define PRIMITIV_CUDA_MEMORY_POOL_H_

#include <cstdint>
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace primitiv {

class CUDAMemoryDeleter;

/**
 * Memory manager on the CUDA devices.
 */
class CUDAMemoryPool {
  friend CUDAMemoryDeleter;

  CUDAMemoryPool() = delete;
  CUDAMemoryPool(const CUDAMemoryPool &) = delete;
  CUDAMemoryPool(CUDAMemoryPool &&) = delete;
  CUDAMemoryPool &operator=(const CUDAMemoryPool &) = delete;
  CUDAMemoryPool &operator=(CUDAMemoryPool &&) = delete;

public:
  /**
   * Creates a memory pool.
   * @param device_id CUDA Device ID on which memories are stored.
   */
  explicit CUDAMemoryPool(unsigned device_id);

  ~CUDAMemoryPool();

  /**
   * Allocates a memory.
   * @param size Size of the resulting memory.
   * @return Shared pointer of the allocated memory.
   */
  std::shared_ptr<void> allocate(std::uint64_t size);

private:
  /**
   * Checks whether given pool object is alive or not.
   * @param pool Reference of a CUDAMemoryPool object.
   * @return true if pool is alive, false otherwise.
   */
  static bool is_alive(const CUDAMemoryPool &pool) {
    return pools_.find(&pool) != pools_.end();
  }

  /**
   * Disposes the memory.
   * @param ptr Handle of the memory to be disposed.
   */
  void free(void *ptr);

  /**
   * Releases all reserved memory blocks.
   */
  void release_reserved_blocks();

  static std::unordered_set<const CUDAMemoryPool *> pools_;
  unsigned dev_id_;
  std::vector<std::vector<void *>> reserved_;
  std::unordered_map<void *, unsigned> supplied_;
};

/**
 * Custom deleter class for CUDA memories.
 */
class CUDAMemoryDeleter {
  CUDAMemoryDeleter() = delete;
public:
  explicit CUDAMemoryDeleter(CUDAMemoryPool &pool) : pool_(pool) {}
  void operator()(void *ptr) {
    if (CUDAMemoryPool::is_alive(pool_)) {
      pool_.free(ptr);
    }
  }
private:
  CUDAMemoryPool &pool_;
};

}  // namespace primitiv

#endif  // PRIMITIV_CUDA_MEMORY_POOL_H_
