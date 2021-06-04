# marc/os - NEM Software
#Find the native Symbolcore headers and libraries, sourced at https://github.com/nemtech/symbol-sdk-core-cpp
#
#Usage:
#	FindPackage(SYMBOLCORE)
#
#Defines:
#	SYMBOLCORE_INCLUDE_DIRS   - where to find
#	SYMBOLCORE_LIBRARIES      - List of libraries when using it.
#	SYMBOLCORE_FOUND          - True if found.

find_path(SYMBOLCORE_INCLUDE_DIR symbol/core/core.h
	/usr/include
	/usr/local/include
	DOC	"Directory containing symbolcore headers"
)
mark_as_advanced(SYMBOLCORE_INCLUDE_DIR)

find_library( SYMBOLCORE_LIBRARY
	NAMES	symbolcore
	PATHS	/usr/lib
		/usr/local/lib
		/usr/lib/i386-linux-gnu/
		/usr/lib/x86_64-linux-gnu/
)
mark_as_advanced(SYMBOLCORE_LIBRARY)

if ( NOT SYMBOLCORE_INCLUDE_DIR OR NOT SYMBOLCORE_LIBRARY )
	if ( SYMBOLCORE_REQUIRED )
		message( FATAL_ERROR "symbolcore is required." )
	endif ()
else ()
	set( SYMBOLCORE_FOUND 1 )
	mark_as_advanced( SYMBOLCORE_FOUND )
endif ()

if(SYMBOLCORE_FOUND)
	set(SYMBOLCORE_LIBRARIES ${SYMBOLCORE_LIBRARY})
	set(SYMBOLCORE_INCLUDE_DIRS ${SYMBOLCORE_INCLUDE_DIR})
endif()

