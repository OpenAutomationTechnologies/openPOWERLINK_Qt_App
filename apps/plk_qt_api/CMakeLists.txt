SET(OPLK_QT_WRAP "oplkqtwrap")
PROJECT (${OPLK_QT_WRAP} CXX) 

CMAKE_MINIMUM_REQUIRED (VERSION 2.8.11)

MESSAGE(STATUS "Configuring ${OPLK_QT_WRAP}")
#SET(CMAKE_BUILD_TYPE "Release")

OPTION(CONFIG_OPLK_QT_API_LIB           "Compile openPOWERLINK QT API static library" ON)

IF(CMAKE_SYSTEM_NAME STREQUAL "Windows")
	OPTION(CONFIG_OPLK_QT_API_WINDOWS_DLL   "Build openPOWERLINK Qt API library as DLL" OFF)
ENDIF(CMAKE_SYSTEM_NAME STREQUAL "Windows")

IF(CONFIG_OPLK_QT_API_WINDOWS_DLL)
    SET(LIBRARY_TYPE SHARED)
ELSE()
    SET(LIBRARY_TYPE STATIC)
ENDIF()

FIND_OPLK_LIBRARY("mn")

MESSAGE(STATUS "Searching includes and sources in ${OPLK_QT_API_LIB_INCLUDE_DIR} ${OPLK_QT_API_LIB_SOURCE_DIR}")
FILE ( GLOB_RECURSE LIB_SOURCES "${OPLK_QT_API_LIB_SOURCE_DIR}/*.cpp" )
FILE ( GLOB_RECURSE LIB_HEADERS "${OPLK_QT_API_LIB_INCLUDE_DIR}/*.h" )

INCLUDE_DIRECTORIES(${OPLK_QT_API_LIB_INCLUDE_DIR})

find_package(Qt5Core REQUIRED)
find_package(Qt5Xml REQUIRED)

get_target_property(QtCore_location_Release Qt5::Core LOCATION_Release)
#get_target_property(QtCore_location_Debug Qt5::Core LOCATION_Debug)
#INCLUDE_DIRECTORIES(${QtCore_location})
#message(STATUS "${QtCore_location_Release}  ${QtCore_location_Debug}")

get_filename_component(QT_DLL_DIR ${QtCore_location_Release} PATH)

#get_target_property(QtXml_location_Release Qt5::Xml LOCATION_Release)
#get_target_property(QtXml_location_Debug Qt5::Xml LOCATION_Debug)
#INCLUDE_DIRECTORIES(${QtXml_location})
#message(STATUS "${QtXml_location_Release}  ${QtXml_location_Debug}")


ADD_DEFINITIONS(${QT_DEFINITIONS}
                -DCONFIG_MN
                -DUNICODE
				-DOPLKQTWRAP_LIB)
#                -DQT_XML_LIB
#                -DQT_CORE_LIB

if(NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
    ADD_DEFINITIONS(-DQT_NO_DEBUG)
endif()

# Find includes in corresponding build directories
SET(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
SET(CMAKE_AUTOMOC ON)

ADD_LIBRARY(${OPLK_QT_WRAP} ${LIBRARY_TYPE} ${LIB_SOURCES} ${LIB_HEADERS})

IF(UNIX)
SET_TARGET_PROPERTIES(${OPLK_QT_WRAP} PROPERTIES PREFIX "")
ENDIF()

TARGET_LINK_LIBRARIES(${OPLK_QT_WRAP} optimized ${OPLKLIB_RELEASE} debug ${OPLKLIB_DEBUG})
TARGET_LINK_LIBRARIES(${OPLK_QT_WRAP} Qt5::Core Qt5::Xml ${ADD_ADDITIONAL_DEPENDENCIES})
#TARGET_LINK_LIBRARIES(${OPLK_QT_WRAP} Qt5::Core Qt5::Xml ${ADD_ADDITIONAL_DEPENDENCIES} ${PCAP_LIBRARIES})




SET_PROPERTY(TARGET ${OPLK_QT_WRAP} PROPERTY DEBUG_POSTFIX "_d")


STRING(TOLOWER ${CMAKE_SYSTEM_NAME} CMAKE_SYSTEM_NAME_LOWER)
SET(OPLK_APPS_LIB_INSTALL_PREFIX
    ${OPLK_APPS_ROOT_DIR}/lib/${OPLK_QT_WRAP} CACHE PATH "openPOWERLINK wrapper library install prefix" FORCE
    )

SET(CMAKE_INSTALL_PREFIX ${OPLK_APPS_LIB_INSTALL_PREFIX})

INSTALL(TARGETS ${OPLK_QT_WRAP} 
        RUNTIME DESTINATION ${OPLK_APPS_LIB_INSTALL_PREFIX}
        ARCHIVE DESTINATION ${OPLK_APPS_LIB_INSTALL_PREFIX}
        )

IF(CONFIG_OPLK_QT_API_WINDOWS_DLL)

INSTALL(FILES ${OPLKDLL_RELEASE}
        DESTINATION ${OPLK_APPS_LIB_INSTALL_PREFIX}
        CONFIGURATIONS "Release"
       )

INSTALL(FILES ${OPLKDLL_DEBUG}
        DESTINATION ${OPLK_APPS_LIB_INSTALL_PREFIX}
        CONFIGURATIONS "Debug"
       )

IF(CMAKE_SYSTEM_NAME STREQUAL "Windows")
INSTALL(FILES 
        ${QT_DLL_DIR}/Qt5Xml.dll
        ${QT_DLL_DIR}/Qt5Core.dll
        ${QT_DLL_DIR}/icuin51.dll
        ${QT_DLL_DIR}/icuuc51.dll
        DESTINATION ${OPLK_APPS_LIB_INSTALL_PREFIX}
        CONFIGURATIONS "Release"
       )

INSTALL(FILES 
        ${QT_DLL_DIR}/Qt5Xmld.dll
        ${QT_DLL_DIR}/Qt5Cored.dll
        ${QT_DLL_DIR}/icuin51.dll
        ${QT_DLL_DIR}/icuuc51.dll
        DESTINATION ${OPLK_APPS_LIB_INSTALL_PREFIX}
        CONFIGURATIONS "Debug"
        )
ENDIF(CMAKE_SYSTEM_NAME STREQUAL "Windows")

ENDIF(CONFIG_OPLK_QT_API_WINDOWS_DLL)


#### ---------- DOCUMENTATION

#SET(OPLK_QT_API_DOC_DIR ${OPLK_ROOT_DIR}/doc/examples/demo_qt_advanced)
#FIND_PACKAGE(Doxygen)
#IF(DOXYGEN_FOUND)
#CONFIGURE_FILE(${OPLK_QT_API_DOC_DIR}/doxyfile ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)
#ADD_CUSTOM_TARGET(DOCUMENTATION ALL
#${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
#WORKING_DIRECTORY ${OPLK_QT_API_DOC_DIR}
#COMMENT "Generating API documentation with Doxygen" VERBATIM
#)
#ENDIF(DOXYGEN_FOUND)




















#--------------------------------------------

#SET(PLK_QT_BINARY_DIR "${CMAKE_SOURCE_DIR}/bin")


#IF(${OPLK_QT_MN_GUI})
       # ADD_SUBDIRECTORY(plk_qt_gui)
#ENDIF(${OPLK_QT_MN_GUI})


##### ??? TODO Add oplkmn find configuration.
#SET(OBJDICT CiA302-4_MN)            # Set object dictionary to use
#FIND_OPLK_LIBRARY("mn")             # Find suitable openPOWERLINK library

################################################################################
# Setup the architecture specific definitions
#TODO add after separating files.
#IF(CMAKE_SYSTEM_NAME STREQUAL "Linux")
#    include(linux.cmake)
#ELSEIF(CMAKE_SYSTEM_NAME STREQUAL "Windows")
#    include (windows.cmake)
#ELSE()
#    MESSAGE(FATAL_ERROR "System ${CMAKE_SYSTEM_NAME} is not supported!")
#ENDIF()
