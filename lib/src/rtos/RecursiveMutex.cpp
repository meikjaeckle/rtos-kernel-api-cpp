// Copyright (c) 2024 Meik Jaeckle
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include <cstdint>

#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>

#include <kernelapi/rtos/Ticks.hpp>
#include <kernelapi/rtos/RecursiveMutex.hpp>

namespace rtos
{

RecursiveMutex::RecursiveMutex() :
  _handle{nullptr}
{
  _handle = xSemaphoreCreateRecursiveMutex();
  assert(_handle && "Failed to create the recursive mutex");
}

RecursiveMutex::~RecursiveMutex()
{
  vSemaphoreDelete(_handle);
}

bool RecursiveMutex::IsValid() const
{
  return (nullptr != _handle);
}

bool RecursiveMutex::TryLock()
{
  return TryLockFor(0);
}

bool RecursiveMutex::TryLockFor(rtos::Ticks_t timeout)
{
  const BaseType_t ret = xSemaphoreTakeRecursive(static_cast<QueueHandle_t>(_handle), timeout);
  return (pdTRUE == ret);
}

void RecursiveMutex::Lock()
{
  bool locked {false};

  // Wait until we got the lock.
  // Use while loop in case infinite wait is not implemented by the rtos port.
  do
  {
    locked = TryLockFor(GetMaxDelay());
  }
  while(!locked);
}

void RecursiveMutex::Unlock()
{
  [[maybe_unused]] const BaseType_t ret = xSemaphoreGiveRecursive(static_cast<QueueHandle_t>(_handle));
}

bool RecursiveMutex::IsLocked()
{
  // If not locked, xSemaphoreGetMutexHolder returns NULL
  const TaskHandle_t taskHdl = xSemaphoreGetMutexHolder(static_cast<QueueHandle_t>(_handle));
  return (nullptr != taskHdl);
}

} // namespace rtos
