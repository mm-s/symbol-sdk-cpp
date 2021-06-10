configure_file(config.h.in config.h)

file(GLOB symbol_core_SRC *.cpp)
file(GLOB symbol_core_catapult_SRC "catapult/*.cpp")
file(GLOB symbol_core_hmi_SRC "Hmi/*.cpp")
file(GLOB symbol_core_donna_SRC "donna/*.c") # "donna/*.h")

add_definitions(-DNO_MISALIGNED_ACCESSES) # sha3
#SOURCE_GROUP("donna" FILES ${external_donna_SRC})

#-------------Boost
set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost 1.45.0 REQUIRED COMPONENTS log thread filesystem)

if(NOT Boost_FOUND)
	message(FATAL_ERROR "Boost required.")
endif()

#-------------OpenSSL
find_package(OpenSSL 1.1.1 EXACT REQUIRED)
message("OpenSSL   ver: ${OPENSSL_VERSION}")
message("OpenSSL  root: ${OPENSSL_ROOT_DIR}")
message("OpenSSL   inc: ${OPENSSL_INCLUDE_DIR}")
message("OpenSSL  libs: ${OPENSSL_LIBRARIES}")


#------------- libsymbolcore.so
add_library(symbolcore SHARED ${symbol_core_SRC}
		       ${symbol_core_catapult_SRC}
		       ${symbol_core_donna_SRC}
		       ${symbol_core_hmi_SRC}
)

#-------------Use OpenSSL, Boost
include_directories(${OPENSSL_INCLUDE_DIR})
target_link_libraries(symbolcore OpenSSL::SSL)

add_compile_definitions("BOOST_ALL_DYN_LINK")
include_directories(${Boost_INCLUDE_DIRS})
target_link_libraries(symbolcore ${Boost_LIBRARIES})

set_property(TARGET symbolcore PROPERTY POSITION_INDEPENDENT_CODE ON)

target_include_directories(symbolcore
  INTERFACE
    # Headers used from source/build location:
    "${PROJECT_BINARY_DIR}/core"
    "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>"
    # Headers used from installed location:
    "$<INSTALL_INTERFACE:include>"
)
install(TARGETS symbolcore DESTINATION lib)

install(DIRECTORY "${CMAKE_SOURCE_DIR}/core/" # source directory
        DESTINATION "include/symbol/core" # target directory
        FILES_MATCHING # install only matched files
        PATTERN "*.h" # select header files
)

install(FILES "${CMAKE_SOURCE_DIR}/include/symbol/core.h" DESTINATION "include/symbol/")


