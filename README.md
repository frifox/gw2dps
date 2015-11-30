GW2DPS
=======
- A DPS meter for GW2. Based on rafi's [gw2lib](http://www.gamerevision.com/showthread.php?3691-Gw2lib&p=45709).

Download
=======
- Download & extract latest release [here](https://github.com/frifox/gw2dps/releases) (2015-11-29)
- Run gw2dps.exe
- File > Inject
- Have fun.
- To exit, hit F12 key

Build it yourself
=======
1. Install Git
2. Install Visual Studio 2013
3. Install CMake
4. Setup Boost
	- download [BOOST](http://sourceforge.net/projects/boost/files/boost/1.59.0/boost_1_59_0.zip/download)
	- extract to c:\local so boost root ends up as "c:\local\boost_1_59_0"
	- open cli and run,
	- ```setx BOOST_ROOT "c:\local\boost_1_59_0"```
	- ```cd %BOOST_ROOT%```
	- ```"C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\vcvarsall.bat"```
	- ```bootstrap.bat```
	- ```b2 variant=release```
5. Setup wxWidgets
	- download [wxWidgets](http://sourceforge.net/projects/wxwindows/files/3.0.2/wxWidgets-3.0.2.zip/download)
	- extract to c:\local, so the wxwidgets root ends up as "c:\local\wxWidgets-3.0.2"
	- open cli and run,
	- ```"C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\vcvarsall.bat"```
	- ```setx WXWIDGET_ROOT "C:\local\wxWidgets-3.0.2"```
	- ```cd %WXWIDGET_ROOT%\build\msw```
	- ```nmake -f makefile.vc BUILD=release```
6. Setup the project
	- make a dir where you'll store the files, ie "c:\hacklib"
	- open cli and run,
	- ```cd c:\hacklib```
	- ```git clone https://bitbucket.org/rafzi/hacklib.git .```
	- ```git clone https://bitbucket.org/rafzi/hacklib_gw2.git ./src/hacklib_gw2```
	- ```git clone https://github.com/frifox/gw2dps.git ./src/gw2dps```
	- ```rd /s /q src\disableGfx src\injector src\veh_benchmark```
	- ```cmake .```
7. Build
	- open c:\hacklib\hacklib_project.sln (should auto open with VS2013)
	- Swap build to Release
	- Build > Build Solution
	- compiled binaries will be found in c:\hacklib\bin folder

Notes
=======
- Before launching GW2DPS switch GW2 to "Windowed"/"Windowed Fullscreen" mode (Options->Graphics->Resolution). Once GW2DPS is running, feel free to switch back to Fullscreen. 
- Some reported SweetFX caususing compatiblity issues. If it's causing crashes, switch it off.
- If you have a 64 bit (x64) version of MS Visual C++ 2013 Redistributable installed, uninstall it. 
- If app is complaining about missing MSVCP120d.dll, install vsredist_x86.exe.
- If app is complaining about missing dx3dx9_43.dll, install dxwebsetup.exe.
- both vsredist_x86 and dxwebsetup can be found here: https://github.com/frifox/gw2dps/releases/tag/redist