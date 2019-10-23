./build.ps1 4x4Backpack:testbench1:master -verbose
./build.ps1 4x4Backpack:testbench2:master -verbose
./flash_serial.bat 4x4Backpack testbench1 master COM4
./flash_serial.bat 4x4Backpack testbench1 master COM26
