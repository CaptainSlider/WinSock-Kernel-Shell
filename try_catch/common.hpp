#pragma once
#include <cstdint>
#include <ntddk.h>
#include <type_traits>
#include <intrin.h>
#include "delete_constructors.hpp"

namespace hh::common
{
    inline constexpr int pool_tag = 'WSHA';
  // RAII spinlock.
  class spinlock_guard : non_copyable
  {
  private:
    static constexpr uint32_t max_wait_ = 65536;
    volatile long* lock_;

  private:
    bool try_lock() noexcept;
    void lock_spinlock() noexcept;
    void unlock() noexcept;

  public:
    spinlock_guard(spinlock_guard&&);
    spinlock_guard& operator=(spinlock_guard&&);
    spinlock_guard(volatile long* lock) noexcept;
    ~spinlock_guard() noexcept;
  };

  // RAII irql guard.
  class irql_guard : non_relocatable
  {
  private:
    KIRQL old_irql_;

  public:
    irql_guard(KIRQL new_irql) noexcept;
    ~irql_guard() noexcept;
  };

  // Set - Unset bit
#define BITS_PER_LONG (sizeof(UINT32) * 8)
#define BITMAP_ENTRY(nr, bmap) ((bmap))[(nr) / BITS_PER_LONG]
#define BITMAP_SHIFT(nr) ((nr) % BITS_PER_LONG)

  // Translate virtual address to physical.
  uint64_t virtual_address_to_physical_address(void* virtual_address) noexcept;
  // Translate physical address to virtual.
  uint64_t physical_address_to_virtual_address(uint64_t physical_address) noexcept;
  // Set chosen bit.
  void set_bit(void* address, uint64_t bit, bool set) noexcept;
  // Get chosen bit.
  uint8_t get_bit(void* address, uint64_t bit) noexcept;
  // Get cr3 of the system process.
  uint64_t find_system_directory_table_base() noexcept;
  constexpr uint64_t compile_time_log2(uint64_t x)
  {
    return x == 1 ? 0 : 1 + compile_time_log2(x / 2);
  }
}
