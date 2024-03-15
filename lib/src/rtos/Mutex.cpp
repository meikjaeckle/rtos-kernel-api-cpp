// Copyright (c) 2024 Meik Jaeckle
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include <cstdint>

#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>

#include <kernelapi/rtos/Ticks.hpp>
#include <kernelapi/rtos/Mutex.hpp>

namespace rtos
{

Mutex::Mutex() :
  _handle{nullptr}
{
  _handle = xSemaphoreCreateMutex();
  assert(_handle && "Failed to create the mutex");
}

Mutex::~Mutex()
{
  vSemaphoreDelete(_handle);
}

bool Mutex::IsValid() const
{
  return (nullptr != _handle);
}

bool Mutex::TryLock()
{
  return TryLockFor(0);
}

bool Mutex::TryLockFor(rtos::Ticks_t timeout)
{
  const BaseType_t ret = xSemaphoreTake(static_cast<QueueHandle_t>(_handle), timeout);
  return (pdTRUE == ret);
}

void Mutex::Lock()
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

void Mutex::Unlock()
{
  [[maybe_unused]] const BaseType_t ret = xSemaphoreGive(static_cast<QueueHandle_t>(_handle));
}

bool Mutex::IsLocked()
{
  // If not locked, xSemaphoreGetMutexHolder returns NULL
  const TaskHandle_t taskHdl = xSemaphoreGetMutexHolder(static_cast<QueueHandle_t>(_handle));
  return (nullptr != taskHdl);
}

} // namespace rtos
