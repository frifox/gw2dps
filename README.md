GW2DPS
=======

DPS meter for GW2. Binaries are in the bin folder. Based on rafi's [gw2lib](http://www.gamerevision.com/showthread.php?3691-Gw2lib&p=45709).

Setup
=======
Install Visual Studio 2013
Install Boost (See Boost section for more info)
Install Git (add git to you PATH environment variable)
Install CMake
clone this repo with git (git clone https://github.com/Cubimon/gw2dps.git)
call bootstrap.bat (this will clone rafzi's lib and setup the folder structure)
now the project is inside hacklib/src/gw2dps

Building
=======
call from the hacklib root directory:
"cmake --build . --config Release" to build the release version or
"cmake --build . --config" to build the debug version or
import the project into visual studio with the *.sln file in the hacklib folder.
(you have compile Boost libs again if you change from debug to release or release to debug)

Boost v1.56.0
=======
Download Boost 1.56.0 from Sourceforge (url) and unzip.
create a new environment variable with the key BOOST_ROOT and as value the path to Boost's root folder.
(e.g. from command line "setx BOOST_ROOT C:/User/Cubimon/libs/Boost/")
Then call from command line at the boost root folder:
```
bootstrap.bat
```

if you want to create the debug version:
```
b2 variant=debug
```
for the release otherwise:
```
b2 variant=release
```

Hotkeys
=======
```
Reference Screen (Alt /)

DPS Meter (Alt D)
DPS History (Alt Shift D)

Kill Timer (Alt NumPad7)
Kill Timer Average DPS (Alt NumPad1)

Monitor Hits (Alt NumPad8)
Record Hits to File (gw2exe's folder) (Alt NumPad5)
Show Hits History (Alt NumPad2)

Monitor Attack Rates (Alt NumPad9)
Record Attack Rates to File (gw2exe's folder) (Alt NumPad6)
Show Attack Rate History (Alt NumPad3)
Adjust Attack Rate Threshold (Alt PgUp/PgDown)

Nearby Ally Players List (Alt C)
Adjust WvW Health Bonus (Alt Home/End)

Count Ally NPCs (Alt 1)
Count Enemy NPCs (Alt 2)
Count Ally Players (Alt 3)
Count Enemy Players (Alt 4)
Show Floaters for the Counted (Alt F)
Show Distance / Max HP for Floaters (Alt Shift F)
Adjust Count Radius (Alt NumPad-/NumPad+)

Speedometer (Alt 9)
Speedometer lock on Self/Enemy (Alt Shift 9)
Distance Measurement (Alt 0)
Distance lock on Self/Enemy (Alt Shift 0)
```

----------------------------------

Notes
=======

- Make sure GW2 keybinds aren't conflicting with the ones I hardcoded above.
- Before launching GW2DPS switch GW2 to "Windowed"/"Windowed Fullscreen" mode (Options->Graphics->Resolution). Once GW2DPS is running, feel free to switch back to Fullscreen. 
- Some reported SweetFX caususing compatiblity issues. If it's causing crashes, switch it off.
- If you have a 64 bit (x64) version of MS Visual C++ 2013 Redistributable installed, uninstall it. 
- If app is complaining about missing MSVCP120d.dll, install vsredist_x86.exe from the dist folder.
- If app is complaining about missing dx3dx9_43.dll, install dxwebsetup.exe from the dist folder.
