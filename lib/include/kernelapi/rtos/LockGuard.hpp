// Copyright (c) 2024 Meik Jaeckle
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#ifndef HEADER_3D994825025C4BFB87A874447C3B6B84
#define HEADER_3D994825025C4BFB87A874447C3B6B84

#include <kernelapi/rtos/Ticks.hpp>

namespace rtos
{

struct DeferLock {};
struct TryToLock {};

constexpr DeferLock deferLock = {};
constexpr TryToLock tryToLock = {};

/**
 * @brief The class LockGuard is a mutex wrapper that provides a convenient RAII-style mechanism for owning a mutex for the duration of a scoped block.
 *        When a LockGuard object is created, it attempts to take ownership of the mutex it is given. When control leaves the scope in
 *        which the LockGuard object was created, the LockGuard is destructed and the mutex is released.
 *        The LockGuard class is non-copyable.
 * @note The LockGuard was inspired by the C++11 implementations of lock_guard, unique_lock and shared_lock.

 */
template <typename MUTEX>
class LockGuard
{
  // Make this class non-copyable
  public:
  LockGuard(const LockGuard& other) = delete;
  LockGuard& operator=(const LockGuard& other) = delete;

	public:

  /**
   * Stores a reference to mutex and invokes mutex.Lock() for locking the mutex.
   * @note Does not return before the mutex was locked.
   * @param[in] mutex Reference of the mutex to be stored.
   */
  explicit LockGuard(MUTEX& mutex) :
    _mutex(mutex), _isLocked(false)
  {
    Lock();
  }

  /**
   * Stores a reference to mutex but does not lock the mutex.
   *        The mutex can be locked by the methods Lock(), TryLock() or TryLockFor()
   * @param[in] mutex Reference of the mutex to be stored.
   * @note DeferLock tag parameter is used to select non-locking version of the constructor.
   */
  LockGuard(MUTEX& mutex, DeferLock) noexcept :
    _mutex(mutex), _isLocked(false)
  {}

  /**
   * Stores the reference to the given mutex and tries to lock the mutex.
   * @param[in] mutex Reference of the mutex to be stored.
   * @note TryToLock tag parameter used to select try-lock version of the constructor.
   * @note Returns immediately, even if the mutex was not locked! You can call OwnsLock() to verify if the mutex was locked.
   */
  LockGuard(MUTEX& mutex, TryToLock) :
    _mutex(mutex), _isLocked(false)
  {
    TryLock();
  }

  /**
   * Stores the reference to the given mutex and tries to lock it.
   * Blocks until specified \a timeout has been reached or the lock is acquired.
   * @param[in] mutex Reference of the mutex to be stored.
   * @param[in] timeout maximum time to block for.
   * @note To verify the mutex was locked, call OwnsLock()
   */
  LockGuard(MUTEX& mutex, rtos::Ticks_t timeout) :
    _mutex(mutex), _isLocked(false)
  {
    TryLockFor(timeout);
  }

	/**
	 * Unlocks the mutex, when it owns the lock.
	*/
	~LockGuard()
	{
		Unlock();
	}

	/**
	 * Lock the mutex, wait until mutex is available.
	 */
	void Lock()
	{
		if (!OwnsLock())
		{
			_mutex.Lock();
			_isLocked = true;
		}
	}

	/**
	 * Tries to lock the mutex, does not wait if the mutex is not available.
	 * @returns true if it owns the lock, false otherwise.
	 */
	bool TryLock()
	{
		if (!OwnsLock())
		{
			_isLocked = _mutex.TryLock();
		}

		return _isLocked;
	}

	/**
	 * Tries to lock the mutex.
   * Blocks until specified \a timeout has been reached or the lock is acquired.
   * @param[in] timeout maximum time to block for.
	 * @returns true, if it owns the lock, false otherwise.
	 */
	bool TryLockFor(rtos::Ticks_t timeout)
	{
		if (!OwnsLock())
		{
			_isLocked = _mutex.TryLockFor(timeout);
		}

		return _isLocked;
	}

	/**
	 * Unlock the mutex, if it was locked before.
	 */
	void Unlock() noexcept
	{
		if (OwnsLock())
		{
			_isLocked = false;
			_mutex.Unlock();
		}
	}

	/**
	 * @returns true, when it owns the lock, false otherwise.
	*/
	bool OwnsLock() const noexcept
	{
		return _isLocked;
	}

	private:
	MUTEX& _mutex;
	bool _isLocked;
};

} // namespace rtos

#endif // HEADER_3D994825025C4BFB87A874447C3B6B84
