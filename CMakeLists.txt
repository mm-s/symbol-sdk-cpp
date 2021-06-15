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

set(CMAKE_CXX_FLAGS_DEBUG_INIT "-g -O0")
set(CMAKE_CXX_FLAGS_RELEASE_INIT " -O3")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

#-------------Boost
set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost 1.45.0 REQUIRED COMPONENTS log thread filesystem coroutine)

if(NOT Boost_FOUND)
        message(FATAL_ERROR "Boost required.")
endif()

add_definitions(-DBOOST_BIND_GLOBAL_PLACEHOLDERS -DBOOST_DISABLE_PRAGMA_MESSAGE)

#-------------OpenSSL
find_package(OpenSSL 1.1.1 EXACT REQUIRED)
message("OpenSSL   ver: ${OPENSSL_VERSION}")
message("OpenSSL  root: ${OPENSSL_ROOT_DIR}")
message("OpenSSL   inc: ${OPENSSL_INCLUDE_DIR}")
message("OpenSSL  libs: ${OPENSSL_LIBRARIES}")




add_subdirectory(core)
add_subdirectory(rpc)
add_subdirectory(cli)

install(FILES "${CMAKE_SOURCE_DIR}/cmake/Modules/FindSYMBOLCORE.cmake" DESTINATION "share/cmake/SYMBOLCORE/")
install(FILES "${CMAKE_SOURCE_DIR}/cmake/Modules/FindSYMBOLRPC.cmake" DESTINATION "share/cmake/SYMBOLRPC")



