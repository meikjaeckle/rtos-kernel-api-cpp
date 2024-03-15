// Copyright (c) 2024 Meik Jaeckle
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>

#include <kernelapi/rtos/Ticks.hpp>

namespace rtos
{

Ticks_t GetMaxDelay()
{
  return portMAX_DELAY;
}

} // namespace rtos