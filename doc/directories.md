Directory structure {#page_directories}
===================

[TOC]

The following page describes the directory structure of the openPOWERLINK apps distribution package.

# Main directories {#sect_directories_main}

Directory               | Description
----------------------- | -----------------------------------------------
apps                    | Advanced applications for evaluating the openPOWERLINK stack
bin                     | Installation directory for binaries.
doc                     | Documentation of the openPOWERLINK supporting libraries and applications
lib                     | Installation directory for the openPOWERLINK API wrapper library.


## Apps {#sect_directories_apps}
A set of demo applications is included in the openPOWERLINK distribution. The
demos contain examples of how to implement an MN or CN using the openPOWERLINK
stack.

Directory              | Description
-----------------------| -----------------------------------------------
common                 | Contains common configuration used by all apps
console_app            | Console application which implements a MN.
plk_qt_api             | API wrapper for openPOWERLINK V2.x stack.
plk_qt_mn_gui          | QT based advanced application which implements an MN.



### Oplk_QT_wrapper {#sect_directories_apps_oplkqtwrapper}
A wrapper built for openPOWERLINK stack using the QT 5 framework, which provides
the possibility to develop applications rapid for openPOWERLINK.

Directory              | Description
-----------------------| -----------------------------------------------
include                | Library include files.
src                    | Library source files.
cmake                  | CMake configurations specific to library.
build                  | Platform specific build configurations.

### Oplk_QT_MN_GUI {#sect_directories_apps_oplkqtmngui}
An QT based advanced application which implements an openPOWERLINK MN using the 
Oplk_QT_wrapper library.

Directory              | Description
-----------------------| -----------------------------------------------
include                | GUI include files.
src                    | GUI source files.
cmake                  | CMake configurations specific to GUI.
build                  | Platform specific build configurations.
ui                     | The QT5 user interface designer files are present.