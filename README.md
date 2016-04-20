GW2DPS
=======
- A DPS meter for GW2. Based on rafi's [gw2lib](http://www.gamerevision.com/showthread.php?3691-Gw2lib&p=45709).

Download
=======
- Download & extract latest release [here](https://github.com/frifox/gw2dps/releases)
- Run gw2dps.exe
- File > Inject
- Have fun.
- To exit, hit F12 key

Build it yourself
=======

Note: If Git or CMake asks if they want to add themselves to your PATH environment variable, allow it.

Note2: These instructions have been tested with Visual Studio Community 2013 Update 5 (on Win 7 x64).

1. Install [Git](https://git-scm.com/download/win)
2. Install [Visual Studio 2013](https://www.visualstudio.com/downloads/download-visual-studio-vs#d-community-2013)
3. Install [CMake](https://cmake.org/files/v3.4/cmake-3.4.3-win32-x86.exe)
4. Setup Boost
	- download [BOOST](http://sourceforge.net/projects/boost/files/boost/1.59.0/boost_1_59_0.zip/download)
	- extract to c:\local so boost root ends up as "c:\local\boost_1_59_0"
	- open cli and run,
	- ```setx BOOST_ROOT "c:\local\boost_1_59_0"```
	- ```set BOOST_ROOT=c:\local\boost_1_59_0```
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
	- ```set WXWIDGET_ROOT=C:\local\wxWidgets-3.0.2```
	- ```cd %WXWIDGET_ROOT%\build\msw```
	- ```nmake -f makefile.vc BUILD=release```
6. Setup and Build the project
	- make a dir where you'll store the files, ie "c:\hacklib"
	- open cli and run,
	- ```cd c:\hacklib```
	- ```git clone https://bitbucket.org/rafzi/hacklib.git .```
	- ```git clone https://bitbucket.org/rafzi/hacklib_gw2.git ./src/hacklib_gw2```
	- ```git clone https://github.com/frifox/gw2dps.git ./src/gw2dps```
	- ```rd /s /q src\disableGfx src\injector src\veh_benchmark src\example src\overlay```
	- ```mkdir build && cd build```
	- ```"C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\vcvarsall.bat"```
	- ```cmake ..```
	- ```msbuild hacklib_project.sln /p:Configuration=Release /p:Platform=Win32```
		- to enable parallel build, add **/m:2** to the above command (change 2 to however many cpu cores you have).
	- compiled binaries will be found in c:\hacklib\bin folder

### 64-bit build
1. Prep
	- Follow the instructions above to build 32-bit first.
2.  Build Boost 64-bit
	- open cli and run,
	- ```cd %BOOST_ROOT%```
	- ```"C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\vcvarsall.bat" amd64```
	- ```b2 --stagedir=stage64 variant=release address-model=64```
3. Build wxWidgets 64-bit (optional)
	- open cli and run,
	- ```cd %WXWIDGET_ROOT%\build\msw```
	- ```"C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\vcvarsall.bat" amd64```
	- ```nmake -f makefile.vc BUILD=release TARGET_CPU=x64```
4. Build gw2dps 64-bit
	- open cli and run,
	- ```cd c:\hacklib```
	- ```mkdir build64 && cd build64```
	- ```"C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\vcvarsall.bat" amd64```
	- ```cmake -G"Visual Studio 12 2013 Win64" ..```
	- ```msbuild hacklib_project.sln /p:Configuration=Release /p:Platform=x64```
		- to enable parallel build, add **/m:2** to the above command (change 2 to however many cpu cores you have).
	- ```move ..\bin64\proxy64.exe ..\bin && move ..\bin64\gw2dps64.dll ..\bin```
	- compiled binaries will be found in c:\hacklib\bin folder

Notes
=======
- Some reported SweetFX caususing compatiblity issues. If it's causing crashes, switch it off.
- If app is complaining about missing MSVCP120d.dll, install vsredist_x86.exe.
- If app is complaining about missing dx3dx9_43.dll, install dxwebsetup.exe.
- both vsredist_x86 and dxwebsetup can be found here: https://github.com/frifox/gw2dps/releases/tag/redist
