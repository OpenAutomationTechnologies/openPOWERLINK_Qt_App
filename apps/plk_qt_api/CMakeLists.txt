################################################################################
#
# CMake file of openPOWERLINK API QT wrapper library
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
# Setup project and generic options

SET(OPLK_QT_WRAP "oplkqtwrap")
PROJECT (${OPLK_QT_WRAP} CXX)

CMAKE_MINIMUM_REQUIRED (VERSION 2.8.11)

INCLUDE(${CMAKE_SOURCE_DIR}/../common/cmake/options.cmake)

SET(OPLK_QT_WRAP_SOURCE_DIR ${OPLK_QT_WRAP_DIR}/src)

MESSAGE(STATUS "Configuring ${OPLK_QT_WRAP}")

OPTION(CONFIG_OPLK_QT_WRAP_LIB           "Compile openPOWERLINK QT API static library" ON)
IF(CMAKE_SYSTEM_NAME STREQUAL "Windows")
OPTION(CONFIG_OPLK_QT_WRAP_WINDOWS_DLL   "Build openPOWERLINK Qt API library as DLL" OFF)
ENDIF()

################################################################################
# Setup compile definitions depending on configuration
SET(CONFIG_NODE "mn")
FIND_OPLK_LIBRARY(${CONFIG_NODE})

find_package(Qt5Core REQUIRED)
find_package(Qt5Xml REQUIRED)

# Find includes in corresponding build directories
SET(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
SET(CMAKE_AUTOMOC ON)


################################################################################
# Find library includes and source files
MESSAGE(STATUS "Searching includes and sources in ${OPLK_QT_WRAP_INCLUDE_DIR} ${OPLK_QT_WRAP_SOURCE_DIR}")
FILE ( GLOB_RECURSE LIB_SOURCES "${OPLK_QT_WRAP_SOURCE_DIR}/*.cpp" )
FILE ( GLOB_RECURSE LIB_HEADERS "${OPLK_QT_WRAP_INCLUDE_DIR}/*.h" )

INCLUDE_DIRECTORIES(${OPLK_QT_WRAP_INCLUDE_DIR})

ADD_DEFINITIONS(${QT_DEFINITIONS} -DUNICODE -DPLKQTAPI_LIB)

###############################################################################
# Add a target to generate API documentation with Doxygen
###############################################################################
OPTION (CONFIG_OPLK_QT_WRAP_API_DOCUMENTATION "Build source code documentation." OFF)
IF(CONFIG_OPLK_QT_WRAP_API_DOCUMENTATION)
	FIND_PACKAGE(Doxygen REQUIRED)
	IF(DOXYGEN_FOUND)
		CONFIGURE_FILE(${OPLK_QT_WRAP_LIB_DOC_DIR}/doxyfile ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)
		ADD_CUSTOM_TARGET(
			DOCUMENTATION ALL ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
			WORKING_DIRECTORY ${OPLK_QT_WRAP_LIB_DOC_DIR}
			COMMENT "Generating API documentation with Doxygen" VERBATIM
		)
	ENDIF(DOXYGEN_FOUND)
ENDIF(CONFIG_OPLK_QT_WRAP_API_DOCUMENTATION)


################################################################################
# Setup the architecture specific definitions
IF(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    include(cmake/linux.cmake)
ELSEIF(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    include (cmake/windows.cmake)
ELSE()
    MESSAGE(FATAL_ERROR "System ${CMAKE_SYSTEM_NAME} is not supported!")
ENDIF()

################################################################################
# Setup compiler specific definitions and flags
INCLUDE("${OPLK_APPS_COMMON_DIR}/cmake/configure-compiler.cmake")

IF(CONFIG_OPLK_QT_WRAP_WINDOWS_DLL)
    SET(LIBRARY_TYPE SHARED)
ELSE()
    SET(LIBRARY_TYPE STATIC)
ENDIF()

IF(NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
    ADD_DEFINITIONS(-DQT_NO_DEBUG)
ENDIF()

IF (MSVC)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /w34100 /w34189 /Zm200 ")     # from Qt-Creator build
ENDIF (MSVC)

MESSAGE( STATUS "CMAKE_CXX_FLAGS: " ${CMAKE_CXX_FLAGS} )

################################################################################
# Setup compile definitions depending on configuration
IF (CFG_KERNEL_STACK_DIRECTLINK)
    ADD_DEFINITIONS(-DCONFIG_KERNELSTACK_DIRECTLINK)
ENDIF (CFG_KERNEL_STACK_DIRECTLINK)

################################################################################
# Get library name depending on the configuration
IF(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    IF(CFG_KERNEL_STACK_DIRECTLINK)
        SET(OPLK_QT_WRAP_LIB_NAME ${OPLK_QT_WRAP}-${CONFIG_NODE})
    ELSEIF (CFG_KERNEL_STACK_USERSPACE_DAEMON)
        SET(OPLK_QT_WRAP_LIB_NAME ${OPLK_QT_WRAP}-${CONFIG_NODE}-userintf)
    ELSEIF (CFG_KERNEL_STACK_KERNEL_MODULE)
        SET(OPLK_QT_WRAP_LIB_NAME ${OPLK_QT_WRAP}-${CONFIG_NODE}-kernelintf)
    ENDIF (CFG_KERNEL_STACK_DIRECTLINK)
ELSEIF(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    SET(OPLK_QT_WRAP_LIB_NAME ${OPLK_QT_WRAP}-${CONFIG_NODE})
ELSE()
    MESSAGE(FATAL_ERROR "Unsupported CMAKE_SYSTEM_NAME ${CMAKE_SYSTEM_NAME} or CMAKE_SYSTEM_PROCESSOR ${CMAKE_SYSTEM_PROCESSOR}")
ENDIF()

################################################################################
# Create the library and set properties
ADD_LIBRARY(${OPLK_QT_WRAP_LIB_NAME} ${LIBRARY_TYPE} ${LIB_SOURCES} ${LIB_HEADERS})
SET_PROPERTY(TARGET ${OPLK_QT_WRAP_LIB_NAME} PROPERTY DEBUG_POSTFIX "_d")

################################################################################
# Libraries to link
TARGET_LINK_LIBRARIES(${OPLK_QT_WRAP_LIB_NAME} optimized ${OPLKLIB_RELEASE} debug ${OPLKLIB_DEBUG})
TARGET_LINK_LIBRARIES(${OPLK_QT_WRAP_LIB_NAME} Qt5::Core Qt5::Xml ${ADD_ADDITIONAL_DEPENDENCIES})

################################################################################
# Installation rules
INSTALL(TARGETS ${OPLK_QT_WRAP_LIB_NAME}
        RUNTIME DESTINATION ${OPLK_QT_WRAP_LIB_DIR}
        ARCHIVE DESTINATION ${OPLK_QT_WRAP_LIB_DIR}
        )
