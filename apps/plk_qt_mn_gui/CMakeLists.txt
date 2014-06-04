SET(OPLK_QT_MN_APP "oplkmn_qt_gui")
PROJECT (${OPLK_QT_MN_APP} CXX)

CMAKE_MINIMUM_REQUIRED (VERSION 2.8.11)

INCLUDE(${CMAKE_SOURCE_DIR}/../common/cmake/options.cmake)

FIND_OPLK_LIBRARY("mn")
FIND_OPLK_QT_WRAP_LIBRARY("oplkqtwrap" "mn")

SET(OPLK_QT_MN_APP_INCLUDE_DIR ${OPLK_QT_MN_APP_DIR}/include)
SET(OPLK_QT_MN_APP_SOURCE_DIR ${OPLK_QT_MN_APP_DIR}/src)
SET(OPLK_QT_MN_APP_UI_DIR ${OPLK_QT_MN_APP_DIR}/ui)
SET(OPLK_QT_MN_APP_RESOURCE_DIR ${OPLK_QT_MN_APP_UI_DIR}/resources)

INCLUDE_DIRECTORIES(${OPLK_QT_MN_APP_INCLUDE_DIR})

FILE ( GLOB_RECURSE GUI_SOURCES "${OPLK_QT_MN_APP_SOURCE_DIR}/*.cpp" )
FILE ( GLOB_RECURSE GUI_HEADERS "${OPLK_QT_MN_APP_INCLUDE_DIR}/*.h" )
FILE ( GLOB_RECURSE GUI_UI_FILES "${OPLK_QT_MN_APP_UI_DIR}/*.ui" )
FILE ( GLOB_RECURSE GUI_RESOURCE_FILES "${OPLK_QT_MN_APP_RESOURCE_DIR}/*.qrc" )

find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Gui REQUIRED)
#get_target_property(QtCore_location_Release Qt5::Core LOCATION_Release)
#get_filename_component(QT_DLL_DIR ${QtCore_location_Release} PATH)
#add_definitions(${Qt5Widgets_DEFINITIONS})

# Find includes in corresponding build directories
SET(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
SET(CMAKE_AUTOMOC ON)
INCLUDE_DIRECTORIES( ${CMAKE_CURRENT_BINARY_DIR} )


qt5_wrap_ui(GUI_UI_HEADERS ${GUI_UI_FILES})
source_group("Generated UI Headers" FILES ${GUI_UI_HEADERS})
qt5_add_resources(GUI_RESOURCES ${GUI_RESOURCE_FILES})
set(RESOURCE_ADDED ${GUI_RESOURCES})

ADD_DEFINITIONS(${QT_DEFINITIONS})

IF(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    IF(CFG_KERNEL_STACK_DIRECTLINK)
        SET(OPLK_CFG_NAME directlink)
    ELSEIF (CFG_KERNEL_STACK_USERSPACE_DAEMON)
        SET(OPLK_CFG_NAME userintf)
    ELSEIF (CFG_KERNEL_STACK_KERNEL_MODULE)
        SET(OPLK_CFG_NAME kernelintf)
    ENDIF (CFG_KERNEL_STACK_DIRECTLINK)
ELSEIF(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    SET(OPLK_CFG_NAME directlink)
ELSE()
    MESSAGE(FATAL_ERROR "Unsupported CMAKE_SYSTEM_NAME ${CMAKE_SYSTEM_NAME} or CMAKE_SYSTEM_PROCESSOR ${CMAKE_SYSTEM_PROCESSOR}")
ENDIF()

SET(OPLK_QT_MN_APP_BIN_DIR
    ${OPLK_QT_WRAP_BIN_DIR}/${OPLK_CFG_NAME} CACHE PATH "openPOWERLINK wrapper library install prefix" FORCE
    )

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
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /w34100 /w34189 /Zm200 ")     # from Qt-Creator build :: needed?
ENDIF (MSVC)

MESSAGE( STATUS "CMAKE_CXX_FLAGS: " ${CMAKE_CXX_FLAGS} )

add_executable (${OPLK_QT_MN_APP} ${GUI_UI_HEADERS} ${GUI_SOURCES} ${GUI_HEADERS} ${GUI_RESOURCES})
qt5_use_modules(${OPLK_QT_MN_APP} Core Gui Widgets)

SET_PROPERTY(TARGET ${OPLK_QT_MN_APP} PROPERTY DEBUG_POSTFIX "_d")

TARGET_LINK_LIBRARIES(${OPLK_QT_MN_APP} optimized ${OPLK_QT_WRAP_LIB_RELEASE} debug ${OPLK_QT_WRAP_LIB_DEBUG})
TARGET_LINK_LIBRARIES(${OPLK_QT_MN_APP} optimized ${OPLKLIB_RELEASE} debug ${OPLKLIB_DEBUG})
TARGET_LINK_LIBRARIES(${OPLK_QT_MN_APP} ${PCAP_LIBRARIES} ${OTHER_DEPENDENT_LIBS})

INSTALL(TARGETS ${OPLK_QT_MN_APP}
RUNTIME DESTINATION ${OPLK_QT_MN_APP_BIN_DIR}/
ARCHIVE DESTINATION ${OPLK_QT_MN_APP_BIN_DIR}/
)

