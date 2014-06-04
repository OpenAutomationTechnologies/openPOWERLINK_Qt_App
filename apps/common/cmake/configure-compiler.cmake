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
