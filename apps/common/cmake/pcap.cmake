################################################################################
#
# Script for finding the PCAP library
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
# PCAP specific configurations

UNSET(OTHER_DEPENDENT_LIBS)
UNSET(PCAP_LIBRARIES)

IF(CMAKE_SYSTEM_NAME STREQUAL "Linux")

    IF (CFG_KERNEL_STACK_DIRECTLINK OR CFG_KERNEL_STACK_USERSPACE_DAEMON)
        SET(PCAP_LIBRARIES pcap)
        ADD_DEFINITIONS (-DCONFIG_USE_PCAP)
    ENDIF (CFG_KERNEL_STACK_DIRECTLINK OR CFG_KERNEL_STACK_USERSPACE_DAEMON)

    ################################################################################
    # Add other dependent libs
    SET(OTHER_DEPENDENT_LIBS pthread rt)

ELSEIF(CMAKE_SYSTEM_NAME STREQUAL "Windows")

    IF(CMAKE_CL_64)
        LINK_DIRECTORIES(${OPLK_ROOT_DIR}/contrib/pcap/windows/WpdPack/Lib/x64)
    ELSE ()
        MESSAGE(STATUS "Searching for LIBRARY wpcap in ${OPLK_ROOT_DIR}/contrib/pcap/windows/WpdPack/Lib")
        LINK_DIRECTORIES(${OPLK_ROOT_DIR}/contrib/pcap/windows/WpdPack/Lib)
    ENDIF()

    INCLUDE_DIRECTORIES(${OPLK_ROOT_DIR}/contrib/pcap/windows/WpdPack/Include)

    SET(PCAP_LIBRARIES wpcap iphlpapi)

    ADD_DEFINITIONS(-DWPCAP -DCONFIG_USE_PCAP)

ENDIF()