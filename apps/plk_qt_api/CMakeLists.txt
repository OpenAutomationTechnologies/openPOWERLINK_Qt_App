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

IF(CONFIG_OPLK_QT_WRAP_WINDOWS_DLL)
    SET(LIBRARY_TYPE SHARED)
ELSE()
    SET(LIBRARY_TYPE STATIC)
ENDIF()

SET(CONFIG_NODE "mn")
FIND_OPLK_LIBRARY(${CONFIG_NODE})

MESSAGE(STATUS "Searching includes and sources in ${OPLK_QT_WRAP_INCLUDE_DIR} ${OPLK_QT_WRAP_SOURCE_DIR}")
FILE ( GLOB_RECURSE LIB_SOURCES "${OPLK_QT_WRAP_SOURCE_DIR}/*.cpp" )
FILE ( GLOB_RECURSE LIB_HEADERS "${OPLK_QT_WRAP_INCLUDE_DIR}/*.h" )

INCLUDE_DIRECTORIES(${OPLK_QT_WRAP_INCLUDE_DIR})

find_package(Qt5Core REQUIRED)
find_package(Qt5Xml REQUIRED)
#??? if QT_BINARY_DIR is empty uncomment 
#get_target_property(QtCore_location_Release Qt5::Core LOCATION_Release)
#get_filename_component(QT_DLL_DIR ${QtCore_location_Release} PATH)

ADD_DEFINITIONS(${QT_DEFINITIONS}
                -DUNICODE
                -DPLKQTAPI_LIB)
# This causes the export macro. from QtApiGlobal.h _PLKQTAPI_GLOBAL_H_
#This is not needed                 -DCONFIG_MN
#                -DQT_XML_LIB
#                -DQT_CORE_LIB

################################################################################
# Setup the architecture specific definitions
################################################################################
IF(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    include(cmake/linux.cmake)
ELSEIF(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    include (cmake/windows.cmake)
ELSE()
    MESSAGE(FATAL_ERROR "System ${CMAKE_SYSTEM_NAME} is not supported!")
ENDIF()

################################################################################
# Setup compiler specific definitions and flags
################################################################################
INCLUDE("${OPLK_APPS_COMMON_DIR}/cmake/configure-compiler.cmake")

IF (MSVC)
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /w34100 /w34189 /Zm200 ")     # from Qt-Creator build
ENDIF (MSVC)

MESSAGE( STATUS "CMAKE_CXX_FLAGS: " ${CMAKE_CXX_FLAGS} )


# Setup compile definitions depending on configuration
IF (CFG_KERNEL_STACK_DIRECTLINK)
    ADD_DEFINITIONS(-DCONFIG_KERNELSTACK_DIRECTLINK)
ENDIF (CFG_KERNEL_STACK_DIRECTLINK)

if(NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
    ADD_DEFINITIONS(-DQT_NO_DEBUG)
endif()

# Find includes in corresponding build directories
SET(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
SET(CMAKE_AUTOMOC ON)

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




ADD_LIBRARY(${OPLK_QT_WRAP_LIB_NAME} ${LIBRARY_TYPE} ${LIB_SOURCES} ${LIB_HEADERS})

TARGET_LINK_LIBRARIES(${OPLK_QT_WRAP_LIB_NAME} optimized ${OPLKLIB_RELEASE} debug ${OPLKLIB_DEBUG})
TARGET_LINK_LIBRARIES(${OPLK_QT_WRAP_LIB_NAME} Qt5::Core Qt5::Xml ${ADD_ADDITIONAL_DEPENDENCIES})



SET_PROPERTY(TARGET ${OPLK_QT_WRAP_LIB_NAME} PROPERTY DEBUG_POSTFIX "_d")


STRING(TOLOWER ${CMAKE_SYSTEM_NAME} CMAKE_SYSTEM_NAME_LOWER)
SET(OPLK_APPS_LIB_INSTALL_PREFIX
    ${OPLK_QT_WRAP_LIB_DIR} CACHE PATH "openPOWERLINK wrapper library install prefix" FORCE
    )

#SET(CMAKE_INSTALL_PREFIX ${OPLK_APPS_LIB_INSTALL_PREFIX})

INSTALL(TARGETS ${OPLK_QT_WRAP_LIB_NAME} 
		RUNTIME DESTINATION ${OPLK_APPS_LIB_INSTALL_PREFIX}
		ARCHIVE DESTINATION ${OPLK_APPS_LIB_INSTALL_PREFIX}
		)

