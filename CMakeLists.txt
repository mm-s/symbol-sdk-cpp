cmake_minimum_required(VERSION 3.14)

if (DEFINED ENV{SYMBOLCORE_VERSION})
	set(SYMBOLCORE_VERSION $ENV{SYMBOLCORE_VERSION})
endif()

if (DEFINED ENV{SYMBOLRPC_VERSION})
	set(SYMBOLRPC_VERSION $ENV{SYMBOLRPC_VERSION})
endif()

if (NOT DEFINED SYMBOLCORE_VERSION)
	set(SYMBOLCORE_VERSION 0.1.0)
endif()

if (NOT DEFINED SYMBOLRPC_VERSION)
	set(SYMBOLRPC_VERSION 0.1.0)
endif()

if (NOT DEFINED ONLY_SYMBOLCORE)
	set(ONLY_SYMBOLCORE OFF)
endif()

if (NOT DEFINED BUILD_SYMBOLCLI)
	set(BUILD_SYMBOLCLI ON)
endif()

project(symbol_sdk VERSION ${SYMBOLRPC_VERSION} )
message(STATUS "symbol-sdk version ${PROJECT_VERSION}")

option(ENABLE_CODE_COVERAGE "Enable code coverage" OFF)
option(ENABLE_TESTS "Enable tests" ON)

if (NOT DEFINED CMAKE_CXX_STANDARD)
	set(CMAKE_CXX_STANDARD 20)
endif()

message(STATUS "C++ std version ${CXX_STD_VERSION}")

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

message("ONLY_SYMBOLCORE: ${ONLY_SYMBOLCORE}")
message("BUILD_SYMBOLCLI: ${BUILD_SYMBOLCLI}")

#-------------jsoncpp
find_package(RapidJSON 1.1.0 REQUIRED)

#find_package(PkgConfig REQUIRED)
#pkg_check_modules(JSONCPP jsoncpp)
#link_libraries(${JSONCPP_LIBRARIES})
#pkg_check_modules(RapidJSON rapidjson)

#link_libraries(${RapidJSON_LIBRARIES})

add_subdirectory(core)
if (${ONLY_SYMBOLCORE})
	message (STATUS "Only core selected. Omits rpc and cli components.")
else()
	message (STATUS "Includes core, rpc components.")
	add_subdirectory(rpc)
endif()

if (${BUILD_SYMBOLCLI})
	message (STATUS "build cli.")
	add_subdirectory(cli)
else()
	message (STATUS "Not building cli.")
endif()

if(ENABLE_TESTS)
	find_package(GTest 1.10.0 EXACT REQUIRED)
	add_subdirectory(test)
endif()




