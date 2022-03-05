#ifndef _FABUTILS_H
 #define _FABUTILS_H
#include "gbConfig.h"
#ifdef use_lib_sound_ay8912

//  Created by Fabrizio Di Vittorio (fdivitto2013@gmail.com) - <http://www.fabgl.com>
//  Copyright (c) 2019-2020 Fabrizio Di Vittorio.
//  All rights reserved.
//
//  This file is part of FabGL Library.
//
//  FabGL is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  FabGL is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with FabGL.  If not, see <http://www.gnu.org/licenses/>.
 

//******************************************************************************
//*******************************************************************************
//  FabGL aggressively trimmed down by David Crespo
//  https://github.com/dcrespo3d
//  for using only its SoundGen and SwGenerator components
//  in ZX-ESPectrum emulator project
//  for emulating AY-3-8912 chip
//*******************************************************************************
//*****************************************************************************

#pragma once


#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"

#include <driver/adc.h>
#include <esp_system.h>


namespace fabgl {


// manage IDF versioning
#ifdef ESP_IDF_VERSION
  #define FABGL_ESP_IDF_VERSION_VAL                      ESP_IDF_VERSION_VAL
  #define FABGL_ESP_IDF_VERSION                          ESP_IDF_VERSION
#else
  #define FABGL_ESP_IDF_VERSION_VAL(major, minor, patch) ((major << 16) | (minor << 8) | (patch))
  #define FABGL_ESP_IDF_VERSION                          FABGL_ESP_IDF_VERSION_VAL(0, 0, 0)
#endif



#define GPIO_UNUSED GPIO_NUM_MAX


//////////////////////////////////////////////////////////////////////////////////////////////
// PSRAM_HACK
// ESP32 Revision 1 has following bug: "When the CPU accesses external SRAM through cache, under certain conditions read and write errors occur"
// A workaround is done by the compiler, so whenever PSRAM is enabled the workaround is automatically applied (-mfix-esp32-psram-cache-issue compiler option).
// Unfortunately this workaround reduces performance, even when SRAM is not access, like in VGAXController interrupt handler. This is unacceptable for the interrupt routine.
// In order to confuse the compiler and prevent the workaround from being applied, a "nop" is added between load and store instructions (PSRAM_HACK).

#ifdef ARDUINO
  #ifdef BOARD_HAS_PSRAM
    #define FABGL_NEED_PSRAM_DISABLE_HACK
  #endif
#else
  #ifdef CONFIG_SPIRAM_SUPPORT
    #define FABGL_NEED_PSRAM_DISABLE_HACK
  #endif
#endif

#ifdef FABGL_NEED_PSRAM_DISABLE_HACK
  #define PSRAM_HACK asm(" nop")
#else
  #define PSRAM_HACK
#endif



//////////////////////////////////////////////////////////////////////////////////////////////


// Integer square root by Halleck's method, with Legalize's speedup
int isqrt (int x);


template <typename T>
const T & tmax(const T & a, const T & b)
{
  return (a < b) ? b : a;
}

constexpr auto imax = tmax<int>;


template <typename T>
const T & tmin(const T & a, const T & b)
{
  return !(b < a) ? a : b;
}

constexpr auto imin = tmin<int>;


template <typename T>
const T & tclamp(const T & v, const T & lo, const T & hi)
{
  return (v < lo ? lo : (v > hi ? hi : v));
}

constexpr auto iclamp = tclamp<int>;


template <typename T>
const T & twrap(const T & v, const T & lo, const T & hi)
{
  return (v < lo ? hi : (v > hi ? lo : v));
}


template <typename T>
void tswap(T & v1, T & v2)
{
  T t = v1;
  v1 = v2;
  v2 = t;
}

constexpr auto iswap = tswap<int>;


template <typename T>
T moveItems(T dest, T src, size_t n)
{
  T pd = dest;
  T ps = src;
  if (pd != ps) {
    if (ps < pd)
      for (pd += n, ps += n; n--;)
        *--pd = *--ps;
    else
      while (n--)
        *pd++ = *ps++;
  }
  return dest;
}


// mode: GPIO_MODE_DISABLE,
//       GPIO_MODE_INPUT,
//       GPIO_MODE_OUTPUT,
//       GPIO_MODE_OUTPUT_OD (open drain),
//       GPIO_MODE_INPUT_OUTPUT_OD (open drain),
//       GPIO_MODE_INPUT_OUTPUT
void configureGPIO(gpio_num_t gpio, gpio_mode_t mode);


///////////////////////////////////////////////////////////////////////////////////
// AutoSemaphore

struct AutoSemaphore {
  AutoSemaphore(SemaphoreHandle_t mutex) : m_mutex(mutex) { xSemaphoreTake(m_mutex, portMAX_DELAY); }
  ~AutoSemaphore()                                        { xSemaphoreGive(m_mutex); }
private:
  SemaphoreHandle_t m_mutex;
};

} // end of namespace


#endif
#endif
