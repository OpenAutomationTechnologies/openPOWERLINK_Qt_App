################################################################################
#
# Windows definitions for openPOWERLINK QT MN app
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
# Set architecture specific installation files
IF(CONFIG_OPLK_QT_WRAP_WINDOWS_DLL)
    IF(NOT (${OPLKDLL_RELEASE} STREQUAL "OPLKDLL_RELEASE-NOTFOUND"))
        INSTALL(FILES ${OPLKDLL_RELEASE}
                DESTINATION ${OPLK_QT_MN_APP_BIN_DIR}
                CONFIGURATIONS "Release"
               )
    ENDIF()

    IF(NOT (${OPLKDLL_DEBUG} STREQUAL "OPLKDLL_DEBUG-NOTFOUND"))
        INSTALL(FILES ${OPLKDLL_DEBUG}
                DESTINATION ${OPLK_QT_MN_APP_BIN_DIR}
                CONFIGURATIONS "Debug"
               )
    ENDIF()

    IF(NOT (${OPLK_QT_WRAP_DLL_RELEASE} STREQUAL "OPLK_QT_WRAP_DLL_RELEASE-NOTFOUND"))
        INSTALL(FILES ${OPLK_QT_WRAP_DLL_RELEASE}
                DESTINATION ${OPLK_QT_MN_APP_BIN_DIR}
                CONFIGURATIONS "Release"
               )
    ENDIF()

    IF(NOT (${OPLK_QT_WRAP_DLL_DEBUG} STREQUAL "OPLK_QT_WRAP_DLL_DEBUG-NOTFOUND"))
        INSTALL(FILES ${OPLK_QT_WRAP_DLL_DEBUG}
                DESTINATION ${OPLK_QT_MN_APP_BIN_DIR}
                CONFIGURATIONS "Debug"
               )
    ENDIF()

ENDIF(CONFIG_OPLK_QT_WRAP_WINDOWS_DLL)


get_target_property(QtCore_location_Release Qt5::Core LOCATION_Release)
get_filename_component(QT_DLL_DIR ${QtCore_location_Release} PATH)

INSTALL(FILES
        ${QT_DLL_DIR}/Qt5Gui.dll
        ${QT_DLL_DIR}/Qt5Widgets.dll
        ${QT_DLL_DIR}/Qt5Xml.dll
        ${QT_DLL_DIR}/Qt5Core.dll
        ${QT_DLL_DIR}/icuin51.dll
        ${QT_DLL_DIR}/icuuc51.dll
        ${QT_DLL_DIR}/icudt51.dll
        ${QT_DLL_DIR}/libEGL.dll
        ${QT_DLL_DIR}/libGLESv2.dll
        DESTINATION ${OPLK_QT_MN_APP_BIN_DIR}
        CONFIGURATIONS "Release"
       )
        #<li>wpcap.dll</li>

INSTALL(FILES
        ${QT_DLL_DIR}/Qt5Guid.dll
        ${QT_DLL_DIR}/Qt5Widgetsd.dll
        ${QT_DLL_DIR}/Qt5Xmld.dll
        ${QT_DLL_DIR}/Qt5Cored.dll
        ${QT_DLL_DIR}/libEGLd.dll
        ${QT_DLL_DIR}/libGLESv2d.dll
        ${QT_DLL_DIR}/icuin51.dll
        ${QT_DLL_DIR}/icuuc51.dll
        ${QT_DLL_DIR}/icudt51.dll
        DESTINATION ${OPLK_QT_MN_APP_BIN_DIR}
        CONFIGURATIONS "Debug"
        )


INSTALL(FILES
        ${QT_DLL_DIR}/../plugins/platforms/qwindows.dll
        DESTINATION ${OPLK_QT_MN_APP_BIN_DIR}/platforms
        CONFIGURATIONS "Release"
       )

INSTALL(FILES
        ${QT_DLL_DIR}/../plugins/platforms/qwindowsd.dll
        DESTINATION ${OPLK_QT_MN_APP_BIN_DIR}/platforms
        CONFIGURATIONS "Debug"
       )