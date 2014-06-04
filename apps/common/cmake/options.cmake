################################################################################
#
# Options specific for openPOWERLINK applications running on Linux
#
# Copyright (c) 2014, Kalycito Infotech Pvt. Ltd.,
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#     * Redistributions of source code must retain the above copyright
#       notice, this list of conditions and the following disclaimer.
#     * Redistributions in binary form must reproduce the above copyright
#       notice, this list of conditions and the following disclaimer in the
#       documentation and/or other materials provided with the distribution.
#     * Neither the name of the copyright holders nor the
#       names of its contributors may be used to endorse or promote products
#       derived from this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
# ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
# WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED. IN NO EVENT SHALL COPYRIGHT HOLDERS BE LIABLE FOR ANY
# DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
# (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
# LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
# ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
# SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
################################################################################

################################################################################
# Set global directories
SET(OPLK_APPS_ROOT_DIR ${CMAKE_SOURCE_DIR}/../..)
SET(OPLK_APPS_LIB_DIR ${OPLK_APPS_ROOT_DIR}/lib)
SET(OPLK_APPS_BIN_DIR ${OPLK_APPS_ROOT_DIR}/bin)
SET(OPLK_APPS_DOC_DIR ${OPLK_APPS_ROOT_DIR}/doc)

SET(OPLK_APPS_COMMON_DIR ${OPLK_APPS_ROOT_DIR}/apps/common)

STRING(TOLOWER ${CMAKE_SYSTEM_NAME} CMAKE_SYSTEM_NAME_LOWER)
STRING(TOLOWER ${CMAKE_SYSTEM_PROCESSOR} CMAKE_SYSTEM_PROCESSOR_LOWER)

SET(OPLK_QT_WRAP_DIR ${OPLK_APPS_ROOT_DIR}/apps/plk_qt_api)
SET(OPLK_QT_WRAP_INCLUDE_DIR ${OPLK_QT_WRAP_DIR}/include)
SET(OPLK_QT_WRAP_LIB_DIR ${OPLK_APPS_LIB_DIR}/plk_qt_api/${CMAKE_SYSTEM_NAME_LOWER}_${CMAKE_SYSTEM_PROCESSOR_LOWER})
SET(OPLK_QT_WRAP_LIB_DOC_DIR ${OPLK_APPS_DOC_DIR}/plk_qt_api)

SET(OPLK_QT_MN_APP_DIR ${OPLK_APPS_ROOT_DIR}/apps/plk_qt_mn_gui)
SET(OPLK_QT_MN_APP_DOC_DIR ${OPLK_APPS_DOC_DIR}/plk_qt_mn_gui)

################################################################################
# Configurable input directories
#SET(CONFIG_openPOWERLINK_CUSTOM_LIBSEARH_PATH CACHE PATH "Add the path to openPOWERLINK stack LIBRARIES to find")

IF (CMAKE_SYSTEM_NAME STREQUAL "Windows")
    SET(CONFIG_QT_INSTALL_PATH "C:\\Qt\\Qt5.2.0\\5.2.0\\msvc2010" CACHE PATH "Add the QT installation path as same as the sample")
    SET(CONFIG_openPOWERLINK_ROOT_DIR "D:/steps/openPOWERLINK_v2.0_QT"  CACHE PATH "Add the absolute path to openPOWERLINK stack V2.0 or later")
ELSE()
    SET(CONFIG_QT_INSTALL_PATH "/opt/Qt5.2.1/5.2.1/gcc" CACHE PATH "Add the QT installation path as same as the sample")
    SET(CONFIG_openPOWERLINK_ROOT_DIR "/home/ramakrishnan/Documents/openPOWERLINK_v2.0_QT" CACHE PATH "Add the absolute path to openPOWERLINK stack V2.0 or later")
ENDIF (CMAKE_SYSTEM_NAME STREQUAL "Windows")


IF(NOT EXISTS "${CONFIG_openPOWERLINK_ROOT_DIR}/")
    MESSAGE(FATAL_ERROR "Set the CONFIG_openPOWERLINK_ROOT_DIR to a valid openPOWERLINK stack root path")
ENDIF()

IF(NOT EXISTS "${CONFIG_QT_INSTALL_PATH}/")
    MESSAGE(FATAL_ERROR "Set the QT install path in the 'CONFIG_QT_INSTALL_PATH' variable")
ENDIF()


SET(CMAKE_PREFIX_PATH ${CONFIG_QT_INSTALL_PATH})

SET(OPLK_ROOT_DIR ${CONFIG_openPOWERLINK_ROOT_DIR})
SET(OPLK_INCLUDE_DIR ${OPLK_ROOT_DIR}/stack/include)
SET(OPLK_LIBRARY_DIR "${OPLK_INCLUDE_DIR}/stack/libs/${CMAKE_SYSTEM_NAME_LOWER}/${CMAKE_SYSTEM_PROCESSOR_LOWER}")

INCLUDE_DIRECTORIES (${OPLK_INCLUDE_DIR})
INCLUDE_DIRECTORIES (${OPLK_QT_WRAP_INCLUDE_DIR})


INCLUDE("${OPLK_APPS_COMMON_DIR}/cmake/findoplklib.cmake")

################################################################################
#Find OPLK version number from version.h
IF(OPLK_INCLUDE_DIR)
    set(_VERSION_REGEX "([0-9]+)")
    SET(OPLK_STACK_MAJOR_VERSION "")
    SET(OPLK_STACK_MINOR_VERSION "")
    SET(OPLK_STACK_SUBMINOR_VERSION "")
    file(STRINGS "${OPLK_INCLUDE_DIR}/oplk/version.h" _OPLK_STACK_MJ_CONTENTS REGEX "#define PLK_STACK_MAJOR_VERSION*")
    file(STRINGS "${OPLK_INCLUDE_DIR}/oplk/version.h" _OPLK_STACK_MI_CONTENTS REGEX "#define PLK_STACK_MINOR_VERSION*")
    file(STRINGS "${OPLK_INCLUDE_DIR}/oplk/version.h" _OPLK_STACK_SMI_CONTENTS REGEX "#define PLK_STACK_BUILD_VERSION*")
    string (REGEX MATCH ${_VERSION_REGEX} OPLK_STACK_MAJOR_VERSION ${_OPLK_STACK_MJ_CONTENTS})
    string (REGEX MATCH ${_VERSION_REGEX} OPLK_STACK_MINOR_VERSION ${_OPLK_STACK_MI_CONTENTS})
    string (REGEX MATCH ${_VERSION_REGEX} OPLK_STACK_SUBMINOR_VERSION ${_OPLK_STACK_SMI_CONTENTS})
    MESSAGE(STATUS "POWERLINK found. Version ${OPLK_STACK_MAJOR_VERSION}.${OPLK_STACK_MINOR_VERSION}.${OPLK_STACK_SUBMINOR_VERSION}")
ENDIF (OPLK_INCLUDE_DIR)


################################################################################
# Include CMake modules
SET(CMAKE_MODULE_PATH ${OPLK_APPS_COMMON_DIR}/cmake ${CMAKE_MODULE_PATH})

IF(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    INCLUDE("configure-linux")
ELSEIF(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    INCLUDE("configure-windows")
ELSE()
    MESSAGE(FATAL_ERROR "System ${CMAKE_SYSTEM_NAME} is not supported!")
ENDIF()
