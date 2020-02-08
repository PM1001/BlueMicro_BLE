IF EXIST C:\Users\pierre\Documents\Arduino\hardware\Adafruit\Adafruit_nRF52_Arduino (
set nrf52librarypath=C:\Users\pierre\Documents\Arduino\hardware\Adafruit\Adafruit_nRF52_Arduino
) ELSE (
set nrf52librarypath=%localappdata%\Arduino15\packages\adafruit\hardware\nrf52\0.16.0
)
	
nrfjprog --family NRF52 --recover
nrfjprog --family NRF52 --eraseall
nrfjprog.exe --program %nrf52librarypath%/bootloader/feather_nrf52832/feather_nrf52832_bootloader-0.3.0_s132_6.1.1.hex -f nrf52 --chiperase --reset 
nrfjprog --family NRF52 --reset
%nrf52librarypath%/tools\adafruit-nrfutil\win32\adafruit-nrfutil.exe  --verbose dfu serial -pkg board_tester.ino.zip -p %1 -b 115200 --singlebank