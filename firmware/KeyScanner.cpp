/*
Copyright 2018-2020 <Pierre Constantineau, Julian Komaromy>

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
#include "KeyScanner.h"
// ToDo: There seems to be lots of redundency in data.
// ToDo: consider interrupts or GPIOTE
// ToDo: there must be a better way to debounce
// ToDo: consider multiple boards and the merging of multiple buffers/modifiers and layer requests.
// ToDo: Action Keycodes - Bluetooth commands
// ToDo: Action Keycodes - Reset/DFU commands

//https://forum.arduino.cc/index.php?topic=319795.0
namespace std {
  void __throw_length_error(char const*) {
  }
}

#if DEFAULT_USER_LAYERS_FUNCTION ==  1 
    uint8_t process_user_layers(std::vector<uint8_t> &buffer)
    {
        uint8_t layer = 0;
        for (auto entry : buffer) 
        {
            if (layer<entry){layer=entry;}
        }
        return layer;
    }
#endif

KeyScanner::KeyScanner() {    // Constructor
    ;
}


/**************************************************************************************************************************/
// KEY SCANNING - THIS ROUTINE ONLY TAKES CARE OF THE DEBOUNCING LOGIC FOR EACH KEY
/**************************************************************************************************************************/
bool KeyScanner::scanMatrix(const int& currentState,unsigned long currentMillis, const int& row, const int& col)
{
    // 
#if DIODE_DIRECTION == COL2ROW
        if (currentState == 0 ) //if key pressed
#else
        if (currentState == 1 ) //if key pressed
#endif
        {
            if (timestamps[row][col] > 0) //if key was previously pressed
            {
                if((currentMillis - timestamps[row][col]) >= DEBOUNCETIME)
                {
                    matrix[row][col].press(currentMillis);
                    lastPressed = currentMillis;
                }
                else // not enough debounce time
                {
                    ; // do nothing
                }
            }

            else //if key was NOT previously pressed
            {
                timestamps[row][col] = currentMillis;
            }
        }
        else // key not pressed
        {
            matrix[row][col].clear(currentMillis);
            timestamps[row][col] = 0;
        }
}


/**************************************************************************************************************************/
// Called by callback function when remote data is received
/**************************************************************************************************************************/
void KeyScanner::updateRemoteLayer(uint8_t data0, uint8_t data1, uint8_t data2,uint8_t data3, uint8_t data4, uint8_t data5,uint8_t data6,uint8_t data7, uint8_t data8, uint8_t data9,uint8_t data10,uint8_t data11, uint8_t data12, uint8_t data13,uint8_t data14)
{
remotelayerBuffer.clear();
if (data0>0) {remotelayerBuffer.push_back(data0);}
if (data1>0) {remotelayerBuffer.push_back(data1);}
if (data2>0) {remotelayerBuffer.push_back(data2);}
if (data3>0) {remotelayerBuffer.push_back(data3);}
if (data4>0) {remotelayerBuffer.push_back(data4);}
if (data5>0) {remotelayerBuffer.push_back(data5);}
if (data6>0) {remotelayerBuffer.push_back(data6);}
if (data7>0) {remotelayerBuffer.push_back(data7);}
if (data8>0) {remotelayerBuffer.push_back(data8);}
if (data9>0) {remotelayerBuffer.push_back(data9);}
if (data10>0) {remotelayerBuffer.push_back(data10);}
if (data11>0) {remotelayerBuffer.push_back(data11);}
if (data12>0) {remotelayerBuffer.push_back(data12);}
if (data13>0) {remotelayerBuffer.push_back(data13);}
if (data14>0) {remotelayerBuffer.push_back(data14);}
}

/**************************************************************************************************************************/
// Called by callback function when remote data is received
/**************************************************************************************************************************/
void KeyScanner::updateRemoteReport(uint8_t data0, uint8_t data1, uint8_t data2,uint8_t data3, uint8_t data4, uint8_t data5,uint8_t data6,uint8_t data7, uint8_t data8, uint8_t data9,uint8_t data10,uint8_t data11, uint8_t data12, uint8_t data13,uint8_t data14)
{
    remoteMod=data0;
    remoteReport[0]= data0;
    remoteReport[1]= data1;
    remoteReport[2]= data2;
    remoteReport[3]= data3;
    remoteReport[4]= data4;
    remoteReport[5]= data5;
    remoteReport[6]= data6;
    remoteReport[7]= data7;
    remoteReport[8]= data8;
    remoteReport[9]= data9;
    remoteReport[10]= data10;
    remoteReport[11]= data11;
    remoteReport[12]= data12;
    remoteReport[13]= data13;
    remoteReport[14]= data14;
}


/**************************************************************************************************************************/
void KeyScanner::resetRemoteReport()
{
    remoteReport[0]= 0;
    remoteReport[1]= 0;
    remoteReport[2]= 0;
    remoteReport[3]= 0;
    remoteReport[4]= 0;
    remoteReport[5]= 0;
    remoteReport[6]= 0;
    remoteReport[7]= 0;
    remoteReport[8]= 0;
    remoteReport[9]= 0;
    remoteReport[10]= 0;
    remoteReport[11]= 0;
    remoteReport[12]= 0;
    remoteReport[13]= 0;
    remoteReport[14]= 0;
}


/**************************************************************************************************************************/
void KeyScanner::resetReport() {
    bufferposition = 1;
    currentMod = 0;
    currentReport[0] = 0;
    currentReport[1] = 0;
    currentReport[2] = 0;
    currentReport[3] = 0;
    currentReport[4] = 0;
    currentReport[5] = 0;
    currentReport[6] = 0;
    currentReport[7] = 0;
    currentReport[8] = 0;
    currentReport[9] = 0;
    currentReport[10] = 0;
    currentReport[11] = 0;
    currentReport[12] = 0;
    currentReport[13] = 0;
    currentReport[14] = 0;
}
/**************************************************************************************************************************/
void KeyScanner::resetLayer() {

 /*   currentLayer[0] = 0;
    currentLayer[1] = 0;
    currentLayer[2] = 0;
    currentLayer[3] = 0;
    currentLayer[4] = 0;
    currentLayer[5] = 0;
    currentLayer[6] = 0;
    currentLayer[7] = 0;
    currentLayer[8] = 0;
    currentLayer[9] = 0;
    currentLayer[10] = 0;
    currentLayer[11] = 0;
    currentLayer[12] = 0;
    currentLayer[13] = 0;
    currentLayer[14] = 0;*/


}


/**************************************************************************************************************************/
void KeyScanner::copyRemoteReport()
{
#if BLE_PERIPHERAL == 1  // PERIPHERAL MUST BE HANDLED DIFFERENTLY THAN CENTRAL - OTHERWISE, THE REPORTS WILL JUST KEEP BOUNCING FROM ONE BOARD TO THE OTHER 
    resetReport();
#else
    currentMod = remoteMod;
    //       currentReport[0] = remoteReport[0];
    bufferposition = 1;
    if (remoteReport[1]>0){currentReport[bufferposition] = remoteReport[1]; bufferposition++; }
    if (remoteReport[2]>0){currentReport[bufferposition] = remoteReport[2]; bufferposition++; }
    if (remoteReport[3]>0){currentReport[bufferposition] = remoteReport[3]; bufferposition++; }
    if (remoteReport[4]>0){currentReport[bufferposition] = remoteReport[4]; bufferposition++; }
    if (remoteReport[5]>0){currentReport[bufferposition] = remoteReport[5]; bufferposition++; }
    if (remoteReport[6]>0){currentReport[bufferposition] = remoteReport[6]; bufferposition++; }
    if (remoteReport[7]>0){currentReport[bufferposition] = remoteReport[7]; bufferposition++; }
    if (remoteReport[8]>0){currentReport[bufferposition] = remoteReport[8]; bufferposition++; }
    if (remoteReport[9]>0){currentReport[bufferposition] = remoteReport[9]; bufferposition++; }
    if (remoteReport[10]>0){currentReport[bufferposition] = remoteReport[10]; bufferposition++; }
    if (remoteReport[11]>0){currentReport[bufferposition] = remoteReport[11]; bufferposition++; }
    if (remoteReport[12]>0){currentReport[bufferposition] = remoteReport[12]; bufferposition++; }
    if (remoteReport[13]>0){currentReport[bufferposition] = remoteReport[13]; bufferposition++; }
    if (remoteReport[14]>0){currentReport[bufferposition] = remoteReport[14]; bufferposition++; }
#endif
}
/*
 * loop through the entire matrix, checking for 
 * activated keys and adding the activated ones
 * into a buffer
 */
void KeyScanner::updateBuffer()
{

    localLayer = process_user_layers(layerBuffer);
    activeKeys.clear();
    bool emptyOneshot = false;

    for(int row = 0; row < MATRIX_ROWS; ++row) {
        for (auto& key : matrix[row]) 
        {
          //pair of activation/duration   first is keycode, second is duration.
            auto activation = key.getPair(localLayer);


            if (activation.first >= LAYER_0 && activation.first <= LAYER_F)
            { 
                layerBuffer.push_back(activation.first - 0xF0);
                locallayerBuffer.push_back(activation.first - 0xF0);
                localLayer = process_user_layers(layerBuffer);
                
            }


            if (activation.first != 0) 
            {
                activeKeys.push_back(activation.first);

                /*
                 * define behavior of
                 * toggle and oneshot keys 
                 * respectively
                 *
                 * empty oneshot when a keycode that's before
                 * the modifiers is pressed
                 */
                if (activation.second == 1) 
                {
                    auto it = std::find(toggleBuffer.begin(), toggleBuffer.end(), activation.first);

                    if (it != toggleBuffer.end())
                    {
                        toggleBuffer.erase(it);
                    }
                    else 
                    {
                        toggleBuffer.push_back(activation.first);
                    }


                }
                else if (activation.second == 2)
                {
                    oneshotBuffer.push_back(activation.first);
                }
                else if (activation.first < 0xE0) 
                {
                    emptyOneshot = true;
                }
            }
        }
    }

    /*
     * empty the toggle 
     * buffer into the main buffer and empty the 
     * oneshot buffer if a non-oneshot
     * key has been pressed
     */
    for (auto activation : toggleBuffer) 
    {
        activeKeys.push_back(activation);
    }
    if (emptyOneshot) 
    {
        for (auto activation : oneshotBuffer) 
        {
            activeKeys.push_back(activation);
        }

        oneshotBuffer.clear();
    }
}



/**************************************************************************************************************************/
// Scan for layer changes - must do this first.
/**************************************************************************************************************************/
bool KeyScanner::updateLayer()
{
    uint8_t prevlayer = localLayer;     // remember last layer
 
    layerBuffer.clear();
    locallayerBuffer.clear();
    for (auto remotelayer : remotelayerBuffer)
    {
        layerBuffer.push_back(remotelayer);
    }
    
    // read through the matrix and select all of the 
    // currently pressed keys 
    updateBuffer();// function that updates the activeKeys and layers

    layerChanged = (prevlayer != localLayer);
    return layerChanged;
}

/**************************************************************************************************************************/
// Update Mods - can be done before or after rest of matrix
// All 8 modifiers are handled through a 8-bit byte.  This is the standard HID implementation
/**************************************************************************************************************************/
bool KeyScanner::updateModifiers()
{
    bool changed = false;                                // indicates "changed" mods

    for (auto keycode : activeKeys)
    {
        //seperate the keycode into the hid keycode and the additional modifiers
        auto extraModifiers = static_cast<uint8_t>((keycode & 0xFF00) >> 8);
        auto hidKeycode = static_cast<uint8_t>(keycode & 0x00FF);

        //check if the hid keycode contains a modifier
        switch (hidKeycode) { 
            case KC_LCTRL:  currentMod |= 1;   changed = true; break;
            case KC_LSHIFT: currentMod |= 2;   changed = true; break;
            case KC_LALT:   currentMod |= 4;   changed = true; break;
            case KC_LGUI:   currentMod |= 8;   changed = true; break;
            case KC_RCTRL:  currentMod |= 16;  changed = true; break;
            case KC_RSHIFT: currentMod |= 32;  changed = true; break;
            case KC_RALT:   currentMod |= 64;  changed = true; break;
            case KC_RGUI:   currentMod |= 128; changed = true; break;
        }

        //add all of the extra modifiers into the current modifier 
        currentMod |= extraModifiers;
    }

    return changed;
}


/**************************************************************************************************************************/

bool KeyScanner::getReport()
{
    resetReport();
    copyRemoteReport();
    updateLayer();
    //localLayer = 0;
    updateModifiers();

    for (auto keycode : activeKeys) 
    {
        auto hidKeycode = static_cast<uint8_t>(keycode & 0x00FF);

        if (hidKeycode >= KC_A && hidKeycode <= KC_EXSEL)  // select only valid keycodes: drop mofifiers
        {
            currentReport[ bufferposition] = hidKeycode;
            ++bufferposition;
        }

        if (bufferposition == 15)
        {
            bufferposition = 1;
        }
    }

    currentReport[0] = currentMod;
    currentReport[15] = localLayer;

   if(  (currentReport[0] != previousReport[0])
        | (currentReport[1] != previousReport[1])
        | (currentReport[2] != previousReport[2])
        | (currentReport[3] != previousReport[3])
        | (currentReport[4] != previousReport[4])
        | (currentReport[5] != previousReport[5])
        | (currentReport[6] != previousReport[6])
        | (currentReport[7] != previousReport[7])
        | (currentReport[8] != previousReport[8])
        | (currentReport[9] != previousReport[9])
        | (currentReport[10] != previousReport[10])
        | (currentReport[11] != previousReport[11])
        | (currentReport[12] != previousReport[12])
        | (currentReport[13] != previousReport[13])
        | (currentReport[14] != previousReport[14])
        | (currentReport[15] != previousReport[15])
        )
    {reportChanged = true;}
    else
    {reportChanged = false;}

    previousReport[0] = currentReport[0];
    previousReport[1] = currentReport[1];
    previousReport[2] = currentReport[2];
    previousReport[3] = currentReport[3];
    previousReport[4] = currentReport[4];
    previousReport[5] = currentReport[5];
    previousReport[6] = currentReport[6];
    previousReport[7] = currentReport[7];
    previousReport[8] = currentReport[8];
    previousReport[9] = currentReport[9];
    previousReport[10] = currentReport[10];
    previousReport[11] = currentReport[11];
    previousReport[12] = currentReport[12];
    previousReport[13] = currentReport[13];
    previousReport[14] = currentReport[14];
    previousReport[15] = currentReport[15];

    return reportChanged;
}

unsigned long KeyScanner::getLastPressed() 
{
    return lastPressed;
}
/**************************************************************************************************************************/


uint8_t KeyScanner::currentReport[16] = {0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0}; 
uint8_t KeyScanner::remoteReport[16]  = {0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0}; 
uint8_t KeyScanner::previousReport[16] ={0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0};
bool    KeyScanner::layerChanged = false;
bool    KeyScanner::reportChanged = true;
uint8_t KeyScanner::localLayer = 0;
//uint8_t KeyScanner::remoteLayer[16] ={0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0};
//uint8_t KeyScanner::currentLayer[16]={0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0};
uint8_t KeyScanner::previouslayer = 0;
uint8_t KeyScanner::remoteMod = 0;
uint8_t KeyScanner::currentMod = 0;
unsigned long KeyScanner::timestamps[MATRIX_ROWS][MATRIX_COLS]  = {0};
unsigned long KeyScanner::lastPressed = 0;
uint8_t KeyScanner::bufferposition = 0;
//uint8_t KeyScanner::layerMode = 0;

std::vector<uint16_t> KeyScanner::keycodeBuffer {};
std::vector<uint8_t> KeyScanner::layerBuffer {};
std::vector<uint8_t> KeyScanner::remotelayerBuffer {};
std::vector<uint8_t> KeyScanner::locallayerBuffer {};


std::vector<uint16_t> KeyScanner::activeKeys {};
std::vector<uint16_t> KeyScanner::macroBuffer {};
std::vector<uint16_t> KeyScanner::toggleBuffer {};
std::vector<uint16_t> KeyScanner::leaderBuffer {};
std::vector<uint16_t> KeyScanner::oneshotBuffer {};
