Directory structure {#page_directories}
===================

[TOC]

The following page describes the directory structure of the openPOWERLINK apps distribution package.

# Main directories {#sect_directories_main}

Directory                     | Description
----------------------------- | -----------------------------------------------
apps                          | Advanced applications for evaluating the openPOWERLINK stack
bin                           | Installation directory for binaries.
doc                           | Documentation of the openPOWERLINK supporting libraries and applications
lib                           | Installation directory for the openPOWERLINK API wrapper library.


## Apps {#sect_directories_examples}
A set of demo applications is included in the openPOWERLINK distribution. The
demos contain examples of how to implement an MN or CN using the openPOWERLINK
stack.

Directory                     | Description
----------------------------- | -----------------------------------------------
common                        | Contains common configuration used by all apps
console_app                   | Console application which implements a MN.
plk_qt_api                    | API wrapper for openPOWERLINK V2.x stack.
plk_qt_mn_gui                 | QT based advanced application which implements an MN.