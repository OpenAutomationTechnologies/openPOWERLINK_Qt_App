################################################################################
#
# Compiler configuration options for openPOWERLINK applications
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

MESSAGE( STATUS "Initial CMAKE_CXX_FLAGS: " ${CMAKE_CXX_FLAGS} )

IF (MSVC)
    SET(CMAKE_CXX_FLAGS "/nologo /W3 /WX-")                               # General
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /D_CRT_SECURE_NO_WARNINGS ")  # Preprocessor
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Gm- /EHsc ")                 # Code-Generation
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Zc:wchar_t /GR-")            # Language
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Gd /TP /errorReport:queue ") # Advanced

    SET(CMAKE_CXX_FLAGS_DEBUG            "/MDd /Od /Ob0 /GS  /fp:precise /D_DEBUG /Zi")
    SET(CMAKE_CXX_FLAGS_RELEASE          "/MD  /O2 /Ob2 /GS- /fp:fast    /DNDEBUG")
    SET(CMAKE_CXX_FLAGS_RELWITHDEBINFO   "/MD  /O2 /Ob2 /GS- /fp:fast    /DNDEBUG /Zi")
    SET(CMAKE_CXX_FLAGS_MINSIZEREL       "/MD  /O1 /Ob1 /GS- /fp:fast    /DNDEBUG")
ELSEIF (CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic -Weffc++ -std=c++03")
    SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG")
ELSE()
    MESSAGE(FATAL_ERROR "Unsupported compiler")
ENDIF (MSVC)


SET(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE INTERNAL
    "Available Build Configurations" FORCE)

IF(NOT CMAKE_GENERATOR MATCHES "Visual Studio")
    IF(NOT CMAKE_BUILD_TYPE)
      SET(CMAKE_BUILD_TYPE Release CACHE STRING
          "Choose the type of build, options are: None Debug Release"
          FORCE)
    ENDIF()
ENDIF()

IF (WIN32)
    ADD_DEFINITIONS (-DWIN32)
ENDIF(WIN32)
