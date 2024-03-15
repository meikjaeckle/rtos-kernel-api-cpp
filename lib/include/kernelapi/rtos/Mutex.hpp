// Copyright (c) 2024 Meik Jaeckle
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#ifndef HEADER_CE2B7FA614264CF0B6C4409A6D665B52
#define HEADER_CE2B7FA614264CF0B6C4409A6D665B52

#include <kernelapi/rtos/Ticks.hpp>

namespace rtos
{

/**
 * @brief The Mutex class is a synchronization primitive that can be used to protect shared data from being simultaneously accessed by multiple tasks.
 *        The Mutex includes a priority inheritance mechanism for eliminating unbounded priority inversion.
 *
 * @note The Mutex class is a wrapper class for the native RTOS mutex implementation..
 *       See https://www.freertos.org/a00113.html for details on the mutex implementation of FreeRTOS.
 *
 * @note  This Mutex class is non-recursive:
 *        From FreeRTOS docu (https://www.freertos.org/xSemaphoreCreateRecursiveMutex.html):
 *          A non-recursive mutex can only be 'taken' by a task once. Any attempt by a task to take a non-recursive mutex that it
 *          already holds will fail - and the mutex will always be given back the first time the task 'gives' the mutex.
 *          If you need to call a mutex recursive within a task, use RecursiveMutex class instead.
 *
 *
 * @note Do not call the mutex methods within an ISR!
 */
class Mutex
{
  // Make this class non-copyable
  public:
  Mutex(const Mutex& other) = delete;
  Mutex& operator=(const Mutex& other) = delete;

  public:
  /**
   * The default constructor.
   * Creates the native RTOS mutex and stores the handle to it.
   * @note Call IsValid() to verify if the mutex was created successfully.
   */
  Mutex();

  /**
   * @brief Take and destroy the mutex handle.
   */
  virtual ~Mutex();

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

#endif // HEADER_CE2B7FA614264CF0B6C4409A6D665B52
