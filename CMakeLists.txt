cmake_minimum_required(VERSION 3.14)

if (DEFINED ENV{SYMBOLRPC_VERSION})
    set(SYMBOLRPC_VERSION $ENV{SYMBOLRPC_VERSION})
endif()

if (NOT DEFINED SYMBOLRPC_VERSION)
    set(SYMBOLRPC_VERSION 0.0.1)
endif()

project(symbol_sdk_rpc VERSION ${SYMBOLRPC_VERSION} )
message(STATUS "Building symbol_sdk_rpc version ${PROJECT_VERSION}")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

add_subdirectory(src)
add_subdirectory(cli)

install(FILES "${CMAKE_SOURCE_DIR}/cmake/Modules/FindSYMBOLRPC.cmake" DESTINATION "share/cmake/SYMBOLRPC")

