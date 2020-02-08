nrfjprog --family NRF52 --recover
nrfjprog --family NRF52 --eraseall

IF EXIST C:\Users\pierre\Documents\Arduino\hardware\Adafruit\Adafruit_nRF52_Arduino (
set nrf52librarypath=C:\Users\pierre\Documents\Arduino\hardware\Adafruit\Adafruit_nRF52_Arduino
) ELSE (
set nrf52librarypath=%localappdata%\Arduino15\packages\adafruit\hardware\nrf52\0.16.0
)

nrfjprog.exe --program %nrf52librarypath%/bootloader/pca10056/pca10056_bootloader-0.3.0_s140_6.1.1.hex -f nrf52 --chiperase --reset 

%nrf52librarypath%/tools/adafruit-nrfutil/win32/adafruit-nrfutil.exe --verbose dfu serial -pkg nrf52840_tester.ino.zip -p COM50 -b 115200 --singlebank 

