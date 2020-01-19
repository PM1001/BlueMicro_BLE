/*
Copyright 2018 <Pierre Constantineau>

3-Clause BSD License

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR 
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/
#ifndef FIRMWARE_H
#define FIRMWARE_H
#include <bluefruit.h>
#include <Adafruit_LittleFS.h>
#include <InternalFileSystem.h>
#include "wiring_digital_gpio.h"
#undef min
#undef max
#include "bluetooth_config.h"
#include "firmware_config.h"
#include "hardware_variants.h"
#include "keyboard_config.h"
#include "avr_mapping.h"
#include "KeyScanner.h"
#include "flash.h"
#include "sleep.h"
#include "bluetooth.h"
#include "battery.h"
#include "LedPwm.h"
#include "LedRGB.h"
#include "gpio.h"
#include "keymap.h"

void setupMatrix(void);
void scanMatrix(void);
void sendKeyPresses(void);


void keyscan();
void processmenu();


#ifndef USER_MACRO_FUNCTION 
#define USER_MACRO_FUNCTION   0 
#define DEFAULT_USER_MACRO_FUNCTION   1 
void process_user_macros(uint32_t macroid);
#else  
#define DEFAULT_USER_MACRO_FUNCTION   0
#endif


#endif /* FIRMWARE_H */
