## openPOWERLINK API QT wrapper library

Project description here
One who wants to build a GUI for POWERLINK shall use this library which makes job more easier.
======================================

[TOC]


### Package dependencies to compile
#### openPOWERLINK stack v2.x
An open-source [POWERLINK][2] protocol stack.

* Get the openPOWERLINK stack from [github][7].


#### CMake
 * For building the library [CMake][5] is used. 
 * A CMake version 2.8.11 or higher is required!

	##### Out-of-Source Builds
	 * CMake supports out-of-source builds. Therefore, all generated files are 
	   located in a seperate build directory. This keeps your sources clean.

#### Compiler
 * Windows: Microsoft visual C++ 2010 or higher
 * Linux: G++ v4.6.3 or higher

#### Qt 5 framework
[Qt][3] is a cross-platform application and UI framework for developers using C++.
The Qt framework can be downloaded from [here][4]. Download the installer 
specific to your platform.

 * Windows: Download and install Qt 5.2.x for Windows
 * Linux: Download and install Qt 5.2.x for Linux

-------

### Build the wrapper library

#### Build the openPOWERLINK stack
Please refer to "Building openPOWERLINK" section from [openPOWERLINK doc site][6]
to build the stack libraries.

#### Compilation (any platform) using `cmake-gui`
 - Set the source path to apps/plk_qt_api
 - Set the build path to apps/plk_qt_api/build/<platform-specific>
 - Click configure
	- The configuration will fail for the first time. 
	- Set the openPOWERLINK stack root path to `CONFIG_openPOWERLINK_ROOT_DIR` of CMake configuration.
		- eg: `CONFIG_openPOWERLINK_ROOT_DIR` = D:/openPOWERLINK_V2/
	- If QT installation path is not found; set the QT install path `CONFIG_QT_INSTALL_PATH` of CMake configuration.
		- eg: `CONFIG_QT_INSTALL_PATH` = C:/Qt/Qt5.2.1/5.2.1/msvc2010
	- Click configure to activate the changes.
- Choose the type of stack libraries from `CONFIG_openPOWERLINK_BUILD_TYPE`
	- windows
		- Link to application
	- Linux
		- Link to application
		- Link to user interface
		- Link to kernel stack daemon
- Click configure to activate the changes.
- The CMake scripts will find the openPOWERLINK libraries.
- Follow platform specific documentation below.

##### Linux
- Finally click generate button to generate the Makefiles. 
  (On Linux, CMake generates Makefiles by default).
- Go the chosen build path from the command prompt.

		` cd apps/plk_qt_api/build/<platform-specific> `

- Create the library using the command.

		`make clean && make install`
- The libraries will be available in `lib/oplkqtwrap/<platform>/
- The user can generate the debug libraries by changing the 
  `CMAKE_BUILD_TYPE[TODO check]` to "Debug".
- The debug libraries are built with "_d" at the end of the library names.

##### Windows
- Choose `CONFIG_OPLK_QT_WRAP_WINDOWS_DLL` to build the library as dll.
- Finally click generate to generate the Visual C++ project files.
- Go the choosen build path.
- Open the `oplkqtwrap.sln` with Visual C++
- Build the INSTALL project to generate the library.
- The libraries will be available in `lib/oplkqtwrap/<platform>/
- The name of the libraries depends on for which stack it is built for.
  Refer to `CONFIG_openPOWERLINK_BUILD_TYPE`
- You can switch between debug and release builds easily from the Visual C++ 
  solution configurations.
- The debug libraries are built with "_d" at the end of the library name.

-------

#### Runtime dependencies
The library is dependent on the openPOWERLINK 2.x and QT 5.2.x libraries. 
Here are the list of dependent libraries.

##### Windows
 - oplkmn-*(depends on choosen stack configuration choosen)* present in stack/libs/*
 - wpcap/libpcap - PCAP library
 - Qt5Core.dll
 - Qt5Xml.dll
 - icuin51.dll
 - icuuc51.dll

##### Linux
 - oplkmn-*(depends on choosen stack configuration choosen)* present in stack/libs/*
 - wpcap/libpcap - PCAP library
 - Qt5Core.so
 - Qt5Xml.so
 - icuin51.so
 - icuuc51.so


-------

#### Wrapper library's API documentation

The API documentation shall be found by building the doxygen file present in
`doc/plk_qt_api/doxygen` and the document shall be accessed from `doc/plk_qt_api/html/index.html`



-------

[2]: http://sourceforge.net/projects/openpowerlink/
[3]: http://qt-project.org
[4]: http://download.qt-project.org/official_releases/qt/5.2
[5]: http://www.cmake.org
[6]: http://openpowerlink.sourceforge.net
[7]: https://github.com/Ramakrishnan45/openPOWERLINK_v2.0_QT