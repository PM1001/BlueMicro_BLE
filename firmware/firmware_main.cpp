/*
Copyright 2018 <Pierre Constantineau, Julian Komaromy>

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
/**************************************************************************************************************************/

#include "firmware.h"
using namespace Adafruit_LittleFS_Namespace;
/**************************************************************************************************************************/
// Keyboard Matrix
byte rows[] MATRIX_ROW_PINS;        // Contains the GPIO Pin Numbers defined in keyboard_config.h
byte columns[] MATRIX_COL_PINS;     // Contains the GPIO Pin Numbers defined in keyboard_config.h  
KeyScanner keys;
bool isReportedReleased = true;
uint32_t run_command_page_bluemicro = 0;

uint32_t loop_counter_pwm = DEFAULT_LOOP_COUNTER_PWM;
uint32_t loop_counter_rgb = DEFAULT_LOOP_COUNTER_RGB;
uint32_t loop_counter_battery = DEFAULT_LOOP_COUNTER_BATTERY;
uint32_t loop_counter_commands = DEFAULT_LOOP_COUNTER_COMMANDS;
uint32_t loop_counter_save_to_flash = DEFAULT_LOOP_COUNTER_SAVE2FLASH;

Flash persisteddata;

/**************************************************************************************************************************/
// put your setup code here, to run once:
/**************************************************************************************************************************/
void setup() {

  #if CFG_DEBUG
    Serial.begin(115200);
    while ( !Serial ) delay(10);   // for nrf52840 with native usb
  #endif
  
  LOG_LV1("BLEMIC","Starting %s" ,DEVICE_NAME);
  persisteddata.begin();
  setupGpio();                                                                // checks that NFC functions on GPIOs are disabled.
  setupBluetooth();

  #if BACKLIGHT_PWM_ON == 1 //setup PWM module
    setupPWM();
  #endif
  #if WS2812B_LED_ON == 1 //setup PWM module
    setupRGB();
  #endif
  // Set up keyboard matrix and start advertising
  setupKeymap();
  setupMatrix();
  startAdv(); 
};
/**************************************************************************************************************************/
//
/**************************************************************************************************************************/
void setupMatrix(void) {
    //inits all the columns as INPUT
   for (const auto& column : columns) {
      LOG_LV2("BLEMIC","Setting to INPUT Column: %i" ,column);
      pinMode(column, INPUT);
    }

   //inits all the rows as INPUT_PULLUP
   for (const auto& row : rows) {
      LOG_LV2("BLEMIC","Setting to INPUT_PULLUP Row: %i" ,row);
      pinMode(row, INPUT_PULLUP);
    }
};
/**************************************************************************************************************************/
// Keyboard Scanning
/**************************************************************************************************************************/
void scanMatrix() {
  uint32_t pindata0 = 0;
  uint32_t pindata1 = 0;
  for(int j = 0; j < MATRIX_ROWS; ++j) {                             
    //set the current row as OUPUT and LOW
    pinMode(rows[j], OUTPUT);
    #if DIODE_DIRECTION == COL2ROW                                         
    digitalWrite(rows[j], LOW);                                       // 'enables' a specific row to be "low" 
    #else
    digitalWrite(rows[j], HIGH);                                       // 'enables' a specific row to be "HIGH"
    #endif
    //loops thru all of the columns
    for (int i = 0; i < MATRIX_COLS; ++i) {
          #if DIODE_DIRECTION == COL2ROW                                         
          pinMode(columns[i], INPUT_PULLUP);                              // 'enables' the column High Value on the diode; becomes "LOW" when pressed 
          #else
          pinMode(columns[i], INPUT_PULLDOWN);                              // 'enables' the column High Value on the diode; becomes "LOW" when pressed
          #endif
    }
      // need for the GPIO lines to settle down electrically before reading.
      #ifdef NRFX_H__  // Added to support BSP 0.9.0
         nrfx_coredep_delay_us(DEFAULT_SETTLING_DELAY);
      #else            // Added to support BSP 0.8.6
        nrf_delay_us(DEFAULT_SETTLING_DELAY);
      #endif

      #ifdef NRF52840_XXAA
        pindata0 = NRF_P0->IN;                                         // read all pins at once
        pindata1 = NRF_P1->IN;                                         // read all pins at once
        for (int i = 0; i < MATRIX_COLS; ++i) {
          if (columns[i]<32) //TODO: need to check on this 32 offset
          {
            KeyScanner::scanMatrix((pindata0>>(columns[i]))&1, millis(), j, i);       // This function processes the logic values and does the debouncing 
          } else
          {
            KeyScanner::scanMatrix((pindata1>>(columns[i]-32))&1, millis(), j, i);    // This function processes the logic values and does the debouncing TODO: need to check on this 32 offset
          }
          pinMode(columns[i], INPUT);                                     //'disables' the column that just got looped thru
        }        
      #else
        pindata0 = NRF_GPIO->IN;                                         // read all pins at once
        for (int i = 0; i < MATRIX_COLS; ++i) {
          KeyScanner::scanMatrix((pindata0>>(columns[i]))&1, millis(), j, i);       // This function processes the logic values and does the debouncing
          pinMode(columns[i], INPUT);                                     //'disables' the column that just got looped thru
        }
      #endif
    pinMode(rows[j], INPUT);                                          //'disables' the row that was just scanned
   }                                                                  // done scanning the matrix
};
/**************************************************************************************************************************/
// Communication with computer and other boards
/**************************************************************************************************************************/
void sendKeyPresses() {
   KeyScanner::getReport();                                            // get state data - Data is in KeyScanner::currentReport  
   if (!(KeyScanner::reportEmpty))  //any key presses anywhere?
   {                                                                              
        sendKeys();
        isReportedReleased = false;
       LOG_LV1("MXSCAN","SEND: %x %x %x %x %x %x %x %x %x %x" ,millis(),KeyScanner::currentReport[0], KeyScanner::currentReport[1],KeyScanner::currentReport[2],KeyScanner::currentReport[3], KeyScanner::currentReport[4],KeyScanner::currentReport[5], KeyScanner::currentReport[6],KeyScanner::currentReport[7] );        
    }
   else                                                                  //NO key presses anywhere
   {
    if ((!isReportedReleased)){
      sendRelease();  
      isReportedReleased = true;                                         // Update flag so that we don't re-issue the message if we don't need to.
      LOG_LV1("MXSCAN","RELEASED: %x %x %x %x %x %x %x %x %x %x" ,millis(),KeyScanner::currentReport[0], KeyScanner::currentReport[1],KeyScanner::currentReport[2],KeyScanner::currentReport[3], KeyScanner::currentReport[4],KeyScanner::currentReport[5], KeyScanner::currentReport[6],KeyScanner::currentReport[7] ); 
    }
   }
  #if BLE_PERIPHERAL ==1   | BLE_CENTRAL ==1                            /**************************************************/
    if(KeyScanner::layerChanged)                                               //layer comms
    {   
        sendlayer(KeyScanner::localLayer);
        LOG_LV1("MXSCAN","Layer %i  %i" ,millis(),KeyScanner::localLayer);
        KeyScanner::layerChanged = false;                                      // mark layer as "not changed" since last update
    } 
  #endif                                                                /**************************************************/
}
/**************************************************************************************************************************/
// put your main code here, to run repeatedly:
/**************************************************************************************************************************/
void loop() {

    keyscan(); // call every time...

    if (loop_counter_commands == 0)
    {
      loop_counter_commands = DEFAULT_LOOP_COUNTER_COMMANDS;
      processmenu(); // call every DEFAULT_LOOP_COUNTER_COMMANDS
    }
    else
    {
      loop_counter_commands--;
      unsigned long timesincelastkeypress = millis() - KeyScanner::getLastPressed();
        if (loop_counter_pwm == 0)
        {
          loop_counter_pwm = DEFAULT_LOOP_COUNTER_PWM;
              #if BACKLIGHT_PWM_ON == 1
                updatePWM( timesincelastkeypress);
              #endif // call every DEFAULT_LOOP_COUNTER_PWM
        }
        else
        {
          loop_counter_pwm--;
                  if (loop_counter_rgb == 0)
                  {
                    loop_counter_rgb = DEFAULT_LOOP_COUNTER_RGB;
                    #if WS2812B_LED_ON == 1  
                    updateRGB(timesincelastkeypress);
                    #endif// call every DEFAULT_LOOP_COUNTER_RGB
                  }
                  else
                  {
                    loop_counter_rgb--;
                        if (loop_counter_save_to_flash == 0)
                        {
                            loop_counter_save_to_flash = DEFAULT_LOOP_COUNTER_SAVE2FLASH;
                            persisteddata.commit();
                        }
                            else
                            {
                              loop_counter_save_to_flash--;
                                if (loop_counter_battery == 0)
                                {
                                  loop_counter_battery = DEFAULT_LOOP_COUNTER_BATTERY;
                                  #if BLE_LIPO_MONITORING == 1
                                    updateBattery();
                                  #endif// call every DEFAULT_LOOP_COUNTER_BATTERY
                                }
                                else
                                {
                                  loop_counter_battery--;  
                                }
                            }       
                  }
        }
    }

    sd_power_mode_set(NRF_POWER_MODE_LOWPWR);
    sd_app_evt_wait();
    delay (HIDREPORTINGINTERVAL);

}; 

/**************************************************************************************************************************/
// put your key scanning code here, to run repeatedly:
/**************************************************************************************************************************/
void keyscan() {
    #if MATRIX_SCAN == 1
    scanMatrix();
  #endif
  #if SEND_KEYS == 1
    sendKeyPresses();    // how often does this really run?
  #endif
   unsigned long timesincelastkeypress = millis() - KeyScanner::getLastPressed();

  #if SLEEP_ACTIVE == 1
    gotoSleep(timesincelastkeypress,Bluefruit.connected());
  #endif

  #if BLE_CENTRAL == 1  
    if ((timesincelastkeypress<10)&&(!Bluefruit.Central.connected()&&(!Bluefruit.Scanner.isRunning())))
    {
      Bluefruit.Scanner.start(0);                                                     // 0 = Don't stop scanning after 0 seconds  ();
    }
  #endif
}


void processmenu()
{
  switch(run_command_page_bluemicro)
  {
    // CORE BLUEMICRO FUNCTIONS
    case RESET:
      NVIC_SystemReset();
    break;  
    case DEBUG:
      ;
    break;   
    case SERIAL_DFU:
       enterSerialDfu();
    break;
    case DFU:
       enterOTADfu();
    break;
    case CLEAR_BONDS:
      Bluefruit.clearBonds();
      Bluefruit.Central.clearBonds();
    break;
    case EEPROM_RESET:
      InternalFS.format();  // using formatting instead of clearbonds due to the potential issue with corrupted file system and the keybord being stuck not being able to pair and save bonds.
    break;
    // USB/BLE FUNCTIONS - OUT_AUTO - OUT_USB - OUT_BT
#if BACKLIGHT_PWM_ON == 1
    // BACKLIGHT FUNCTIONS
    case BL_TOGG:
      stepPWMMode();
    break;
    case BL_STEP:  // step through modes
      stepPWMMode();
    break;
    case BL_ON:
      setPWMMode(3);
      PWMSetMaxVal();
    break;
    case BL_OFF:
      setPWMMode(0);
    break;
    case BL_INC:
      incPWMMaxVal();
    break;
    case BL_DEC:
      decPWMMaxVal();
    break;
    case BL_BRTG:
      setPWMMode(1);
    break;
    case BL_REACT:
      setPWMMode(2);
      PWMSetMaxVal();
    break;
    case BL_STEPINC:
        incPWMStepSize();
      break;
    case BL_STEPDEC:
        decPWMStepSize();
      break;

#endif
#if WS2812B_LED_ON == 1
    // RGB FUNCTIONS
    case RGB_TOG:
      
    break;
    case RGB_MOD:
      
    break;
    case RGB_RMOD:
      
    break;
    case RGB_HUI:
      
    break;
    case RGB_HUD:
      
    break;
    case RGB_SAI:
      
    break;
    case RGB_SAD:
      
    break;
    case RGB_VAI:
      
    break;
    case  RGB_VAD:
      
    break;
    case RGB_M_P:
      
    break;
    case RGB_M_B:
      
    break;
    case RGB_M_R:
      
    break;
    case RGB_M_SW:
      
    break;
    case RGB_M_SN:
      
    break;
    case RGB_M_K:
      
    break;
    case RGB_M_X:
      
    break;
    case  RGB_M_G:
      
    break;
    case RGB_M_T:
      
    break;
    case RGB_SPI:
      
    break;
    case RGB_SPD:
      
    break;
#endif
    default:
       process_user_macros(run_command_page_bluemicro); // call macros
    break;        
  }
  run_command_page_bluemicro = 0;
};

#if DEFAULT_USER_MACRO_FUNCTION == 1
void process_user_macros(uint32_t macroid)
{
  ; // default macro function that's empty.
}
#endif
//********************************************************************************************//
//* Idle Task - runs when there is nothing to do                                             *//
//* Any impact of placing code here on current consumption?                                  *//
//********************************************************************************************//
void rtos_idle_callback(void) {
  // Don't call any other FreeRTOS blocking API()
  // Perform background task(s) here
    sd_power_mode_set(NRF_POWER_MODE_LOWPWR);
    sd_app_evt_wait();  // puts the nrf52 to sleep when there is nothing to do.  You need this to reduce power consumption. (removing this will increase current to 8mA)
};

