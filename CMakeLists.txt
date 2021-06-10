cmake_minimum_required(VERSION 3.14)
if (DEFINED ENV{SYMBOLCORE_VERSION})
    set(SYMBOLCORE_VERSION $ENV{SYMBOLCORE_VERSION})
endif()

if (NOT DEFINED SYMBOLCORE_VERSION)
    set(SYMBOLCORE_VERSION 0.0.3)
endif()

if (DEFINED ENV{SYMBOLRPC_VERSION})
    set(SYMBOLRPC_VERSION $ENV{SYMBOLRPC_VERSION})
endif()

if (NOT DEFINED SYMBOLRPC_VERSION)
    set(SYMBOLRPC_VERSION 0.0.1)
endif()

project(symbol_sdk VERSION ${SYMBOLRPC_VERSION} )
message(STATUS "Building symbol_sdk version ${PROJECT_VERSION}")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

add_subdirectory(core)
add_subdirectory(rpc)
add_subdirectory(cli)

install(FILES "${CMAKE_SOURCE_DIR}/cmake/Modules/FindSYMBOLCORE.cmake" DESTINATION "share/cmake/SYMBOLCORE/")
install(FILES "${CMAKE_SOURCE_DIR}/cmake/Modules/FindSYMBOLRPC.cmake" DESTINATION "share/cmake/SYMBOLRPC")



