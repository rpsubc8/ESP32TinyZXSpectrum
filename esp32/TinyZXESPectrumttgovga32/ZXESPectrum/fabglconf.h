#ifndef _FABGL_CONF_H
 #define _FABGL_CONF_H
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
 

//*****************************************************************************
//*******************************************************************************
//  FabGL aggressively trimmed down by David Crespo
//  https://github.com/dcrespo3d
//  for using only its SoundGen and SwGenerator components
//  in ZX-ESPectrum emulator project
//  for emulating AY-3-8912 chip
//*******************************************************************************
//*****************************************************************************

#pragma once

//Defines ESP32 XTAL frequency in Hz.
#define FABGLIB_XTAL 40000000

#endif
#endif
