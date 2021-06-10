configure_file(config.h.in config.h)

file(GLOB symbol_SRC *.cpp)
file(GLOB symbol_dto_SRC "dto/*.cpp")

#-------------Boost
set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost 1.45.0 REQUIRED COMPONENTS log thread filesystem coroutine)

if(NOT Boost_FOUND)
	message(FATAL_ERROR "Boost required.")
endif()

#-------------OpenSSL
find_package(OpenSSL 1.1.1 EXACT REQUIRED)
message("OpenSSL   ver: ${OPENSSL_VERSION}")
message("OpenSSL  root: ${OPENSSL_ROOT_DIR}")
message("OpenSSL   inc: ${OPENSSL_INCLUDE_DIR}")
message("OpenSSL  libs: ${OPENSSL_LIBRARIES}")

#-------------ZLIB
find_package(ZLIB REQUIRED)

#-------------restc-cpp
message("RESTCCPP_PREFIX: ${RESTCCPP_PREFIX}")
find_package (RESTCCPP)
if (NOT RESTCCPP_FOUND)
	message(FATAL_ERROR "restc-cpp required.")
endif()
message("Restc-Cpp  headers: ${RESTCCPP_INCLUDE_DIRS}")
message("           libs: ${RESTCCPP_LIBRARIES}")

#-------------symbolcore
#find_package (SYMBOLCORE)
#if (NOT SYMBOLCORE_FOUND)
#	message(FATAL_ERROR "symbolcore required.")
#endif()
#message("Symbol-Core  headers: ${SYMBOLCORE_INCLUDE_DIRS}")
#message("             libs: ${SYMBOLCORE_LIBRARIES}")



#------------- libsymbolrpc.so
add_library(symbolrpc SHARED ${symbol_SRC}
		       ${symbol_dto_SRC}
)

add_compile_definitions("BOOST_ALL_DYN_LINK")
include_directories(${Boost_INCLUDE_DIRS})
target_link_libraries(symbolrpc ${Boost_LIBRARIES})

include_directories(${RESTCCPP_INCLUDE_DIRS})
target_link_libraries (symbolrpc ${RESTCCPP_LIBRARIES})

#include_directories(${SYMBOLCORE_INCLUDE_DIRS})
#target_link_libraries (symbolrpc ${SYMBOLCORE_LIBRARIES})
target_link_libraries (symbolrpc symbolcore)

target_link_libraries(symbolrpc OpenSSL::SSL)
target_link_libraries(symbolrpc ZLIB::ZLIB)


set_property(TARGET symbolrpc PROPERTY POSITION_INDEPENDENT_CODE ON)


target_include_directories(symbolrpc
  INTERFACE
    # Headers used from source/build location:
    "${PROJECT_BINARY_DIR}/rpc"
    "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>"
    # Headers used from installed location:
    "$<INSTALL_INTERFACE:include>"
)
install(TARGETS symbolrpc DESTINATION lib)

install(DIRECTORY "${CMAKE_SOURCE_DIR}/rpc/" # source directory
        DESTINATION "include/symbol" # target directory
        FILES_MATCHING # install only matched files
        PATTERN "*.h" # select header files
)
