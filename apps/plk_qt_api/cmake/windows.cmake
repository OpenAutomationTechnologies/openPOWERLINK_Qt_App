################################################################################
# Set architecture specific installation files
################################################################################
IF(CONFIG_OPLK_QT_WRAP_WINDOWS_DLL)

    INSTALL(FILES ${OPLKDLL_RELEASE}
        DESTINATION ${CMAKE_PROJECT_NAME}
        CONFIGURATIONS "Release"
           )
    INSTALL(FILES ${OPLKDLL_DEBUG}
        DESTINATION ${CMAKE_PROJECT_NAME}
        CONFIGURATIONS "Debug"
           )

    INSTALL(FILES
        ${QT_BINARY_DIR}/Qt5Xml.dll
        ${QT_BINARY_DIR}/Qt5Core.dll
        ${QT_BINARY_DIR}/icuin51.dll
        ${QT_BINARY_DIR}/icuuc51.dll
        DESTINATION ${CMAKE_PROJECT_NAME}
        CONFIGURATIONS "Release"
           )
    INSTALL(FILES
        ${QT_BINARY_DIR}/Qt5Xmld.dll
        ${QT_BINARY_DIR}/Qt5Cored.dll
        ${QT_BINARY_DIR}/icuin51.dll
        ${QT_BINARY_DIR}/icuuc51.dll
        DESTINATION ${CMAKE_PROJECT_NAME}
        CONFIGURATIONS "Debug"
        )

ENDIF(CONFIG_OPLK_QT_WRAP_WINDOWS_DLL)

