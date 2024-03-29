#define _USE_MATH_DEFINES 
#include "common.hpp"
#include <math.h>

namespace hh::common
{

  spinlock_guard::spinlock_guard(volatile long* lock) noexcept : lock_{ lock }
  {
    lock_spinlock();
  }

  spinlock_guard::~spinlock_guard() noexcept
  {
    unlock();
  }

  void spinlock_guard::lock_spinlock() noexcept
  {
    uint32_t wait = 1;

    while (!try_lock())
    {
      for (uint32_t j = 0; j < wait; j++)
      {
        _mm_pause();
      }

      if (wait * 2 > max_wait_)
      {
        wait = max_wait_;
      }
      else
      {
        wait *= 2;
      }
    }
  }

  bool spinlock_guard::try_lock() noexcept
  {
    return (!(*lock_) && !_interlockedbittestandset(lock_, 0));
  }

  void spinlock_guard::unlock() noexcept
  {
    if(lock_ != nullptr)
    {
      *lock_ = 0;
    }
  }

  spinlock_guard::spinlock_guard(spinlock_guard&& obj)
  {
    lock_ = obj.lock_;
    obj.lock_ = nullptr;
  }

  spinlock_guard& spinlock_guard::operator=(spinlock_guard&& obj)
  {
    unlock();
    lock_ = obj.lock_;
    obj.lock_ = nullptr;

    return *this;
  }

  uint64_t virtual_address_to_physical_address(void* virtual_address) noexcept
  {
    return MmGetPhysicalAddress(virtual_address).QuadPart;
  }

  uint64_t physical_address_to_virtual_address(uint64_t physical_address) noexcept
  {
    PHYSICAL_ADDRESS physical_addr{ .QuadPart = static_cast<long long>(physical_address) };
    return reinterpret_cast<uint64_t>(MmGetVirtualForPhysical(physical_addr));
  }

  void set_bit(void* address, uint64_t bit, bool set) noexcept
  {
    if (set)
    {
      BITMAP_ENTRY(bit, static_cast<uint32_t*>(address)) |= (1UL << BITMAP_SHIFT(bit));
    }
    else
    {
      BITMAP_ENTRY(bit, static_cast<uint32_t*>(address)) &= ~(1UL << BITMAP_SHIFT(bit));
    }
  }

  uint8_t get_bit(void* address, uint64_t bit) noexcept
  {
    uint64_t byte, k;

    byte = bit / 8;
    k = 7 - bit % 8;

    return static_cast<uint8_t*>(address)[byte] & (1 << k);
  }

  irql_guard::irql_guard(KIRQL new_irql) noexcept : old_irql_{ KeGetCurrentIrql() }
  {
    KeLowerIrql(new_irql);
  }

  irql_guard::~irql_guard() noexcept
  {
    KeLowerIrql(old_irql_);
  }
}
