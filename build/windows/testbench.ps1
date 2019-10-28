./build.ps1 4x4Backpack:testbench1:master -verbose
./build.ps1 4x4Backpack:testbench2:master -verbose
./build.ps1 4x4Backpack:testbench3:master -verbose
./build.ps1 4x4Backpack:testbench4:master -verbose
./flash_serial.bat 4x4Backpack testbench3 master COM10

