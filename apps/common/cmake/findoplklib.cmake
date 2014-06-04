################################################################################
#
# Script for finding the openPOWERLINK and openPOWERLINK QT wrapper library
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
# Find the openPOWERLINK library
#    OPLK_NODE_TYPE = mn or cn   (case sensitive)
#    Pre-Requisites :
#                       OPLK_ROOT_DIR
#                       CFG_OPLK_stack_linkage
#
################################################################################
# TODO rename to OPLK_DLL and OPLK_LIB
MACRO(FIND_OPLK_LIBRARY OPLK_NODE_TYPE)

    IF(CMAKE_SYSTEM_NAME STREQUAL "Linux")

        IF(CFG_KERNEL_STACK_DIRECTLINK)
            SET(OPLKLIB_NAME oplk${OPLK_NODE_TYPE})
        ELSEIF (CFG_KERNEL_STACK_USERSPACE_DAEMON)
            SET(OPLKLIB_NAME oplk${OPLK_NODE_TYPE}app-userintf)
        ELSEIF (CFG_KERNEL_STACK_KERNEL_MODULE)
            SET(OPLKLIB_NAME oplk${OPLK_NODE_TYPE}app-kernelintf)
        ELSE()
            SET(OPLKLIB_NAME oplkUnknown)
        ENDIF (CFG_KERNEL_STACK_DIRECTLINK)

    ELSEIF(CMAKE_SYSTEM_NAME STREQUAL "Windows")
        SET(OPLKLIB_NAME oplk${OPLK_NODE_TYPE})
    ELSE()
        MESSAGE(FATAL_ERROR "Unsupported CMAKE_SYSTEM_NAME ${CMAKE_SYSTEM_NAME} or CMAKE_SYSTEM_PROCESSOR ${CMAKE_SYSTEM_PROCESSOR}")
    ENDIF()

    SET(OPLKLIB_DEBUG_NAME "${OPLKLIB_NAME}_d")

    # Set OPLK library and include directory
    SET(OPLKLIB_DIR ${OPLK_ROOT_DIR}/stack/lib/${CMAKE_SYSTEM_NAME_LOWER}/${CMAKE_SYSTEM_PROCESSOR_LOWER})
    SET(OPLKLIB_INCDIR ${OPLK_ROOT_DIR}/stack/proj/${CMAKE_SYSTEM_NAME_LOWER}/lib${OPLKLIB_NAME})


    # Search for release library
    UNSET(OPLKLIB_RELEASE CACHE)
    MESSAGE(STATUS "Searching for LIBRARY ${OPLKLIB_NAME} in ${OPLKLIB_DIR}")
    FIND_LIBRARY(OPLKLIB_RELEASE NAME ${OPLKLIB_NAME} HINTS ${OPLKLIB_DIR} )

    IF(CMAKE_SYSTEM_NAME STREQUAL "Windows")
        UNSET(OPLKDLL_RELEASE CACHE)
        FIND_PROGRAM(OPLKDLL_RELEASE NAME ${OPLKLIB_NAME}.dll HINTS ${OPLKLIB_DIR} )
    ENDIF(CMAKE_SYSTEM_NAME STREQUAL "Windows")

    # Search for debug library
    UNSET(OPLKLIB_DEBUG CACHE)
    MESSAGE(STATUS "Searching for LIBRARY ${OPLKLIB_DEBUG_NAME} in ${OPLKLIB_DIR}")
    FIND_LIBRARY(OPLKLIB_DEBUG NAME ${OPLKLIB_DEBUG_NAME} HINTS ${OPLKLIB_DIR} )

    IF(CMAKE_SYSTEM_NAME STREQUAL "Windows")
        UNSET(OPLKDLL_DEBUG CACHE)
        FIND_PROGRAM(OPLKDLL_DEBUG NAME ${OPLKLIB_DEBUG_NAME}.dll HINTS ${OPLKLIB_DIR} )
    ENDIF(CMAKE_SYSTEM_NAME STREQUAL "Windows")

    INCLUDE_DIRECTORIES(${OPLKLIB_INCDIR})

ENDMACRO(FIND_OPLK_LIBRARY)


################################################################################
# Find the openPOWERLINK QT wrapper library
#    OPLK_NODE_TYPE = mn or cn   (case sensitive)
#    OPLK_QT_WRAP_NAME = oplkqtwrap
#    Pre-Requisites :
#                       OPLK_QT_WRAP_LIB_DIR
#                       OPLK_QT_WRAP_INCLUDE_DIR
#                       CFG_OPLK_stack_linkage
#
################################################################################
MACRO(FIND_OPLK_QT_WRAP_LIBRARY OPLK_QT_WRAP_NAME OPLK_NODE_TYPE)

    IF(CMAKE_SYSTEM_NAME STREQUAL "Linux")
        IF(CFG_KERNEL_STACK_DIRECTLINK)
            SET(OPLK_QT_WRAP_LIB_NAME ${OPLK_QT_WRAP_NAME}-${OPLK_NODE_TYPE})
        ELSEIF (CFG_KERNEL_STACK_USERSPACE_DAEMON)
            SET(OPLK_QT_WRAP_LIB_NAME ${OPLK_QT_WRAP_NAME}-${OPLK_NODE_TYPE}-userintf)
        ELSEIF (CFG_KERNEL_STACK_KERNEL_MODULE)
            SET(OPLK_QT_WRAP_LIB_NAME ${OPLK_QT_WRAP_NAME}-${OPLK_NODE_TYPE}-kernelintf)
        ENDIF (CFG_KERNEL_STACK_DIRECTLINK)
    ELSEIF(CMAKE_SYSTEM_NAME STREQUAL "Windows")
        SET(OPLK_QT_WRAP_LIB_NAME ${OPLK_QT_WRAP_NAME}-${OPLK_NODE_TYPE})
    ELSE()
        MESSAGE(FATAL_ERROR "Unsupported CMAKE_SYSTEM_NAME ${CMAKE_SYSTEM_NAME} or CMAKE_SYSTEM_PROCESSOR ${CMAKE_SYSTEM_PROCESSOR}")
    ENDIF()

    #Set the project name for the library
    SET(OPLK_QT_WRAP_LIB_DEBUG_NAME "${OPLK_QT_WRAP_LIB_NAME}_d")


    # Search for release library
    UNSET(OPLK_QT_WRAP_LIB_RELEASE CACHE)
    MESSAGE(STATUS "Searching for LIBRARY ${OPLK_QT_WRAP_LIB_NAME} in ${OPLK_QT_WRAP_LIB_DIR}")
    FIND_LIBRARY(OPLK_QT_WRAP_LIB_RELEASE NAME ${OPLK_QT_WRAP_LIB_NAME} HINTS ${OPLK_QT_WRAP_LIB_DIR} )

    IF(CMAKE_SYSTEM_NAME STREQUAL "Windows")
        UNSET(OPLK_QT_WRAP_DLL_RELEASE CACHE)
        FIND_PROGRAM(OPLK_QT_WRAP_DLL_RELEASE NAME ${OPLK_QT_WRAP_LIB_NAME}.dll HINTS ${OPLK_QT_WRAP_LIB_DIR} )
        IF(OPLK_QT_WRAP_DLL_RELEASE STREQUAL "OPLK_QT_WRAP_DLL_RELEASE-NOTFOUND")
            #MESSAGE(FATAL_ERROR "${OPLK_QT_WRAP_LIB_NAME}.dll NOT FOUND. Please build oplkQt wrapper release windows dll")
        ENDIF()
    ENDIF(CMAKE_SYSTEM_NAME STREQUAL "Windows")

    # Search for debug library
    UNSET(OPLK_QT_WRAP_LIB_DEBUG CACHE)
    MESSAGE(STATUS "Searching for LIBRARY ${OPLK_QT_WRAP_LIB_DEBUG_NAME} in ${OPLK_QT_WRAP_LIB_DIR}")
    FIND_LIBRARY(OPLK_QT_WRAP_LIB_DEBUG NAME ${OPLK_QT_WRAP_LIB_DEBUG_NAME} HINTS ${OPLK_QT_WRAP_LIB_DIR} )

    IF(CMAKE_SYSTEM_NAME STREQUAL "Windows")
        UNSET(OPLK_QT_WRAP_DLL_DEBUG CACHE)
        FIND_PROGRAM(OPLK_QT_WRAP_DLL_DEBUG NAME ${OPLK_QT_WRAP_LIB_DEBUG_NAME}.dll HINTS ${OPLK_QT_WRAP_LIB_DIR} )
        IF(OPLK_QT_WRAP_DLL_DEBUG STREQUAL "OPLK_QT_WRAP_DLL_DEBUG-NOTFOUND")
            #MESSAGE(FATAL_ERROR "${OPLK_QT_WRAP_LIB_DEBUG_NAME}.dll NOT FOUND. Please build  oplkQt wrapper debug windows dll")
        ENDIF()
    ENDIF(CMAKE_SYSTEM_NAME STREQUAL "Windows")

    INCLUDE_DIRECTORIES(${OPLK_QT_WRAP_INCLUDE_DIR})

    MESSAGE(STATUS "Including the wrapper include dir. ${OPLK_QT_WRAP_INCLUDE_DIR}")

ENDMACRO(FIND_OPLK_QT_WRAP_LIBRARY)
