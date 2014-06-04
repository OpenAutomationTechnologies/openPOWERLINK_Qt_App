################################################################################
# WinPCAP specific configurations
################################################################################
IF(CMAKE_CL_64)
    LINK_DIRECTORIES(${OPLK_ROOT_DIR}/contrib/pcap/windows/WpdPack/Lib/x64)
ELSE ()
    MESSAGE(STATUS "Searching for LIBRARY wpcap in ${OPLK_ROOT_DIR}/contrib/pcap/windows/WpdPack/Lib")
    LINK_DIRECTORIES(${OPLK_ROOT_DIR}/contrib/pcap/windows/WpdPack/Lib)
ENDIF()

INCLUDE_DIRECTORIES(${OPLK_ROOT_DIR}/contrib/pcap/windows/WpdPack/Include)
SET(PCAP_LIBRARIES wpcap iphlpapi)
ADD_DEFINITIONS(-DWPCAP -DCONFIG_USE_PCAP)

################################################################################
# Set architecture specific installation files
################################################################################

IF(CONFIG_OPLK_QT_WRAP_WINDOWS_DLL)
INSTALL(FILES ${OPLKDLL_RELEASE}
        DESTINATION ${OPLK_QT_MN_APP_BIN_DIR}
        CONFIGURATIONS "Release"
       )

INSTALL(FILES ${OPLKDLL_DEBUG}
        DESTINATION ${OPLK_QT_MN_APP_BIN_DIR}
        CONFIGURATIONS "Debug"
       )

INSTALL(FILES ${OPLK_QT_WRAP_DLL_RELEASE}
        DESTINATION ${OPLK_QT_MN_APP_BIN_DIR}
        CONFIGURATIONS "Release"
       )

INSTALL(FILES ${OPLK_QT_WRAP_DLL_DEBUG}
        DESTINATION ${OPLK_QT_MN_APP_BIN_DIR}
        CONFIGURATIONS "Debug"
       )
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
        #<li>platforms/qwindows.dll present in QT_DIR/platforms/</li>

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