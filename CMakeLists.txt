cmake_minimum_required(VERSION 3.4)
project(rnbo.maxapi.getset)


if (APPLE)
    if (${CMAKE_GENERATOR} MATCHES "Xcode")
            if (${XCODE_VERSION} VERSION_LESS 10)
                message(STATUS "Xcode 10 or higher is required. Please install from the Mac App Store.")
                return ()
            elseif(${XCODE_VERSION} VERSION_GREATER_EQUAL 12)
                set(C74_BUILD_FAT YES)
            endif ()
    endif ()

    if (NOT CMAKE_OSX_ARCHITECTURES)
        if(C74_BUILD_FAT)
            set(CMAKE_OSX_ARCHITECTURES "x86_64;arm64" CACHE STRING "macOS architecture" FORCE)
        else()
            set(CMAKE_OSX_ARCHITECTURES ${CMAKE_SYSTEM_PROCESSOR} CACHE STRING "macOS architecture" FORCE)
        endif()
        message("CMAKE_OSX_ARCHITECTURES set to ${CMAKE_OSX_ARCHITECTURES}")
    endif()
else()
	set ( MSVC_COMPILER_NAME "MSVC" )
	set ( PACKAGE_VERSION "1.0.0" )

	if (${CMAKE_CXX_COMPILER_ID} STREQUAL ${MSVC_COMPILER_NAME})
		string (SUBSTRING ${CMAKE_CXX_COMPILER_VERSION} 0 4 MSVC_VERSION_SHORT)
		string (SUBSTRING ${CMAKE_CXX_COMPILER_VERSION} 0 2 MSVC_VERSION_MAJOR)
		string (SUBSTRING ${CMAKE_CXX_COMPILER_VERSION} 3 1 MSVC_VERSION_MINOR)

		if (${MSVC_VERSION_MAJOR} VERSION_LESS 19 OR ${MSVC_VERSION_MAJOR} MATCHES 19 AND ${MSVC_VERSION_MINOR} VERSION_LESS 1)
			# message(STATUS "Visual Studio ${MSVC_VERSION_SHORT} detected. Visual Studio 17 (19.1) or greater is required for UI objects.")
			message(STATUS "Visual Studio 17 or greater is required for UI objects.")
			message(STATUS "SKIPPING!")
			return ()
		else ()
			message(STATUS "Visual Studio ${MSVC_VERSION_SHORT} detected. UI objects will be built.")
		endif ()
	endif ()

	add_definitions(
		-DMAXAPI_USE_MSCRT
		-DWIN_VERSION
		-D_USE_MATH_DEFINES
	)
	
endif()

set(C74_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/external/")

# Misc setup and subroutines
include(${CMAKE_CURRENT_SOURCE_DIR}/Libraries/min-api/script/min-package.cmake)



# Add the Lib, if it exists
if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/Libraries/min-lib/CMakeLists.txt")
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Libraries/min-lib)
endif ()


# Comment the line below if you want automatic cmake regneration enabled
set(CMAKE_SUPPRESS_REGENERATION true)


# Include Min-API
set(C74_MIN_API_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Libraries/min-api)
include(${C74_MIN_API_DIR}/script/min-pretarget.cmake)

#include JUCE
# add_subdirectory("Libraries/JUCE" "JUCE/") 
# set(JUCE_MODULES_ONLY ON)

# only 64-bit, so we don't get problems with modern JUCE
set (C74_X64 ON)


include_directories( 
	"${C74_INCLUDES}"
	"${CMAKE_CURRENT_SOURCE_DIR}"
	"${CMAKE_CURRENT_SOURCE_DIR}/Source"
	"${CMAKE_CURRENT_SOURCE_DIR}/Libraries"
)

set(SOURCE_FILES
	"Source/rnbo.maxapi.getset.cpp"
	"${MAX_SDK_INCLUDES}/common/commonsyms.c"
)

add_library( 
	${PROJECT_NAME} 
	MODULE
	${SOURCE_FILES}
)


include(${C74_MIN_API_DIR}/script/min-posttarget.cmake)