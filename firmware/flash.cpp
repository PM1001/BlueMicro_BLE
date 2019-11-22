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
#include "flash.h"
using namespace Adafruit_LittleFS_Namespace;
File file(InternalFS);

Flash::Flash()
{
  ;
}

void Flash::begin()
{
  InternalFS.begin();
}

void Flash::initialize()
{
    data.blepower = DEVICE_POWER;
    data.pwmmode = 2;
    data.pwmstepsize = 128;
    data.pwmmaxvalue = DEFAULT_PWM_MAX_VALUE;
    data.rgbmode = DEFAULT_RGB_MODE;
    data.rgbhue = DEFAULT_RGB_HUE;
    data.rgbsat = DEFAULT_RGB_SAT;
    data.rgbval = DEFAULT_RGB_VAL;
    writedata();
}

void Flash::readdata()
{
 file.open(FILENAME, FILE_O_READ);
 if ( file ) // file exists
  {
    uint32_t readlen;
    readlen = file.read( &data, sizeof(data));
    file.close();
    saved = true;
  }else
  {
    initialize();
  }
}

void Flash::writedata()
{
    if( file.open(FILENAME, FILE_O_WRITE) )
    {
      file.write((char*) &data, sizeof(data));
      file.close();
      saved = true;
    }else
    {
     // Serial.println("Failed!");
    }
}

void Flash::commit()
{
  if (!saved)
  {
    writedata();
  }
}