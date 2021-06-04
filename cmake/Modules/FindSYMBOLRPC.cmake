# marc/os - NEM Software
#Find the native Symbolcore headers and libraries, sourced at https://github.com/nemtech/symbol-sdk-core-cpp
#
#Usage:
#	FindPackage(SYMBOLRPC)
#
#Defines:
#	SYMBOLRPC_INCLUDE_DIRS   - where to find
#	SYMBOLRPC_LIBRARIES      - List of libraries when using it.
#	SYMBOLRPC_FOUND          - True if found.

find_path(SYMBOLRPC_INCLUDE_DIR symbol/rpc.h
	/usr/include
	/usr/local/include
	DOC "Directory containing symbol headers"
)
mark_as_advanced(SYMBOLRPC_INCLUDE_DIR)

FIND_LIBRARY( SYMBOLRPC_LIBRARY
	NAMES symbolrpc
	PATHS /usr/lib
		/usr/local/lib
		/usr/lib/i386-linux-gnu/
		/usr/lib/x86_64-linux-gnu/
)

mark_as_advanced(SYMBOLRPC_LIBRARY)

if ( NOT SYMBOLRPC_INCLUDE_DIR OR NOT SYMBOLRPC_LIBRARY )
	if ( SYMBOLRPC_REQUIRED )
		message( FATAL_ERROR "symbol is required." )
	endif ()
else ()
	set( SYMBOLRPC_FOUND 1 )
	mark_as_advanced( SYMBOLRPC_FOUND )
endif ()

if(SYMBOLRPC_FOUND)
	set(SYMBOLRPC_LIBRARIES ${SYMBOLRPC_LIBRARY})
	set(SYMBOLRPC_INCLUDE_DIRS ${SYMBOLRPC_INCLUDE_DIR})
endif()
