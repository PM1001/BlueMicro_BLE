param (
    [string]$km = "testbench6",
    [string]$com = "com10"
 )

./build.ps1 4x4Backpack:"$km":master 
./flash_serial.bat 4x4Backpack $km master $com

