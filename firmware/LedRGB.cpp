/*
Copyright 2019 <Pierre Constantineau>

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
#include "LedRGB.h"
#if WS2812B_LED_ON == 1  //setup RGB module

Adafruit_NeoPixel pixels = Adafruit_NeoPixel();


uint16_t hue = DEFAULT_RGB_HUE;  // 0 to 65536
uint8_t sat = DEFAULT_RGB_SAT; // 0 to 255
uint8_t val = DEFAULT_RGB_VAL; // 0 to 255
uint8_t rgbmode = DEFAULT_RGB_MODE;
uint32_t rgbcolor ;

uint32_t colors[WS2812B_LED_COUNT];



void setupRGB(void)
{
   pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
   pixels.setPin(WS2812B_LED_PIN);
   pixels.updateLength(WS2812B_LED_COUNT);
}

void updateRGB( unsigned long timesincelastkeypress)
{

hue = hue + DEFAULT_RGB_HUE_MAX/1024;
if (hue > DEFAULT_RGB_HUE_MAX){hue=0;}

 hue = (uint16_t) millis() >> 2;


pixels.clear();

switch (rgbmode)
{
case 0:                         // OFF
    
    for(uint16_t i=0; i<WS2812B_LED_COUNT; i++) { // For each pixel...
        pixels.setPixelColor(i, 0, 0, 0);  
    }
    break;
case 1:                         // RAINBOW
    
    for(uint16_t i=0; i<WS2812B_LED_COUNT; i++) { // For each pixel...
        
      rgbcolor = pixels.ColorHSV(hue, sat, val);
      pixels.setPixelColor(i,rgbcolor);
    }
    break;
default:
    // unknown mode.  switch to mode 0
    for(int i=0; i<WS2812B_LED_COUNT; i++) { // For each pixel...
         pixels.setPixelColor(i, 0, 0, 0); 
    }
    break;
}



  pixels.show();   // Send the updated pixel colors to the hardware.
}

void suspendRGB(void)
{
    pixels.clear();
  for(int i=0; i<WS2812B_LED_COUNT; i++) { // For each pixel...
    pixels.setPixelColor(i, 0, 0, 0); 
  }
  pixels.show();   // Send the updated pixel colors to the hardware.
}

#endif