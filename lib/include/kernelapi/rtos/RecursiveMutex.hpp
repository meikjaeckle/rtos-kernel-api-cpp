// Copyright (c) 2024 Meik Jaeckle
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#ifndef HEADER_C0465C280F6C424C96B2A59E5D2195B9			   
#define HEADER_C0465C280F6C424C96B2A59E5D2195B9

#include <kernelapi/rtos/Ticks.hpp>

namespace rtos
{

/**
 * @brief The RecursiveMutex class is a synchronization primitive that can be used to protect shared data from being simultaneously accessed by multiple tasks.
 *        The RecursiveMutex includes a priority inheritance mechanism for eliminating unbounded priority inversion.
 *
 * @note The Mutex class is a wrapper class for the native RTOS mutex implementation.
 *       See https://www.freertos.org/a00113.html for details on the mutex implementation of FreeRTOS.
 *       From freeRTOS docu (https://www.freertos.org/xSemaphoreCreateRecursiveMutex.html):
 *         "Contrary to non-recursive mutexes, a task can 'take' a recursive mutex multiple times, and the recursive mutex will only be returned after the holding
 *         task has 'given' the mutex the same number of times it 'took' the mutex."
 *
 * @note Do not call the mutex methods within an ISR!
 */
class RecursiveMutex
{
  // Make this class non-copyable
  public:
  RecursiveMutex(const RecursiveMutex& other) = delete;
  RecursiveMutex& operator=(const RecursiveMutex& other) = delete;

  public:
  /**
   * The default constructor.
   * Creates the native RTOS mutex and stores the handle to it.
   * @note Call IsValid() to verify if the mutex was created successfully.
   */
  RecursiveMutex();

  /**
   * @brief Take and destroy the mutex handle.
   */
  virtual ~RecursiveMutex();

  public:
  /**
   * @returns true if the mutex was created successfully, false otherwise.
   */
  bool IsValid() const;

  /**
   * @brief Locks the mutex.
   * Blocks until mutex is available.
   * @note This method has no timeout, it waits until the mutex is available.
   */
  void Lock();

  /**
   * @brief Tries to lock the mutex, does not block if the mutex is not available.
   * @returns true if it owns the lock, false otherwise.
   */
  bool TryLock();

  /**
   * @brief Tries to lock the mutex.
   * If the mutex is not available, it blocks until specified timeout has been reached.
   * @param[in] timeout maximum time to block until.
   * @returns true, if it owns the lock, false otherwise.
   */
  bool TryLockFor(rtos::Ticks_t timeout);

  /**
   * @brief Unlocks the mutex.
   */
  void Unlock();

  /**
   * @returns true when the mutex is locked
   */
  bool IsLocked();

  private:
  using NativeHdl = void*;
  NativeHdl _handle; /** The native mutex handle managed by an instance of this class. */
};

} // namespace rtos

#endif // HEADER_C0465C280F6C424C96B2A59E5D2195B9
