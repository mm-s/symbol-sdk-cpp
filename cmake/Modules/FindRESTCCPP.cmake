# marc/os - NEM Software
#Find the native restc-cpp headers and libraries, sourced at https://github.com/jgaa/restc-cpp
#
#Usage:
#	FindPackage(RESTCCPP)
#
#Defines:
#	RESTCCPP_INCLUDE_DIRS   - where to find
#	RESTCCPP_LIBRARIES      - List of libraries when using it.
#	RESTCCPP_FOUND          - True if found.

find_path(RESTCCPP_INCLUDE_DIR restc-cpp/SerializeJson.h
	/usr/include
	/usr/local/include
	DOC "Directory containing restc-cpp headers"
)

mark_as_advanced(RESTCCPP_INCLUDE_DIR)

find_library( RESTCCPP_LIBRARY
	NAMES	restc-cpp
	PATHS	/usr/lib
		/usr/local/lib
		/usr/lib/i386-linux-gnu/
		/usr/lib/x86_64-linux-gnu/
)

mark_as_advanced(RESTCCPP_LIBRARY)

if ( NOT RESTCCPP_INCLUDE_DIR OR NOT RESTCCPP_LIBRARY )
	if ( RESTCCPP_REQUIRED )
		message( FATAL_ERROR "restc-cpp is required." )
	endif ()
else ()
	set( RESTCCPP_FOUND 1 )
	mark_as_advanced( RESTCCPP_FOUND )
endif ()

if(RESTCCPP_FOUND)
	set(RESTCCPP_LIBRARIES ${RESTCCPP_LIBRARY})
	set(RESTCCPP_INCLUDE_DIRS ${RESTCCPP_INCLUDE_DIR})
endif()

