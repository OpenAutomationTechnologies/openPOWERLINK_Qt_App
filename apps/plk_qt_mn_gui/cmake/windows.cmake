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

#${CMAKE_PROJECT_NAME} = ${OPLK_APPS_LIB_INSTALL_PREFIX} I hope

IF(CONFIG_OPLK_QT_WRAP_WINDOWS_DLL)
INSTALL(FILES ${OPLKDLL_RELEASE}
        DESTINATION ${OPLK_APPS_BIN_DIR}
        CONFIGURATIONS "Release"
       )

INSTALL(FILES ${OPLKDLL_DEBUG}
        DESTINATION ${OPLK_APPS_BIN_DIR}
        CONFIGURATIONS "Debug"
       )

INSTALL(FILES ${OPLK_QT_WRAP_DLL_RELEASE}
        DESTINATION ${OPLK_APPS_BIN_DIR}
        CONFIGURATIONS "Release"
       )

INSTALL(FILES ${OPLK_QT_WRAP_DLL_DEBUG}
        DESTINATION ${OPLK_APPS_BIN_DIR}
        CONFIGURATIONS "Debug"
       )
ENDIF(CONFIG_OPLK_QT_WRAP_WINDOWS_DLL)


#QT_BINARY_DIR = QT_DLL_DIR ;;;; I hope
INSTALL(FILES
		${QT_BINARY_DIR}/Qt5Gui.dll
		${QT_BINARY_DIR}/Qt5Widgets.dll
        ${QT_BINARY_DIR}/Qt5Xml.dll
        ${QT_BINARY_DIR}/Qt5Core.dll
        ${QT_BINARY_DIRR}/icuin51.dll
        ${QT_BINARY_DIR}/icuuc51.dll
		${QT_BINARY_DIR}/icudt51.dll
		${QT_BINARY_DIR}/libEGL.dll
		${QT_BINARY_DIR}/libGLESv2.dll
        DESTINATION ${OPLK_APPS_BIN_DIR}
        CONFIGURATIONS "Release"
       )
		#<li>wpcap.dll</li>
		#<li>platforms/qwindows.dll present in QT_DIR/platforms/</li>

INSTALL(FILES 
		${QT_BINARY_DIR}/Qt5Guid.dll
		${QT_BINARY_DIR}/Qt5Widgetsd.dll
        ${QT_BINARY_DIR}/Qt5Xmld.dll
        ${QT_BINARY_DIR}/Qt5Cored.dll
		${QT_BINARY_DIR}/libEGLd.dll
		${QT_BINARY_DIR}/libGLESv2d.dll
        ${QT_BINARY_DIR}/icuin51.dll
        ${QT_BINARY_DIR}/icuuc51.dll
		${QT_BINARY_DIR}/icudt51.dll
        DESTINATION ${OPLK_APPS_BIN_DIR}
        CONFIGURATIONS "Debug"
        )
