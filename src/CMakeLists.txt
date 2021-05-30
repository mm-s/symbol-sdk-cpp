configure_file(config.h.in config.h)

file(GLOB symbol_SRC *.cpp)
file(GLOB symbol_dto_SRC "dto/*.cpp")

#-------------Boost
set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost 1.45.0 REQUIRED COMPONENTS log thread filesystem)

if(NOT Boost_FOUND)
	message(FATAL_ERROR "Boost required.")
endif()

#-------------restc-cpp
find_package (RESTCCPP)
if (NOT RESTCCPP_FOUND)
	message(FATAL_ERROR "restc-cpp required.")
endif()


#------------- libsymbolrpc.so
add_library(symbolrpc SHARED ${symbol_SRC}
		       ${symbol_dto_SRC}
)

add_compile_definitions("BOOST_ALL_DYN_LINK")
include_directories(${Boost_INCLUDE_DIRS})
target_link_libraries(symbolrpc ${Boost_LIBRARIES})

include_directories(${RESTCCPP_INCLUDE_DIRS})
target_link_libraries (symbolrpc ${RESTCCPP_LIBRARIES})



set_property(TARGET symbolrpc PROPERTY POSITION_INDEPENDENT_CODE ON)


target_include_directories(symbolrpc
  INTERFACE
    # Headers used from source/build location:
    "${PROJECT_BINARY_DIR}/src"
    "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>"
    # Headers used from installed location:
    "$<INSTALL_INTERFACE:include>"
)
install(TARGETS symbolrpc DESTINATION lib)

install(DIRECTORY "${CMAKE_SOURCE_DIR}/src/" # source directory
        DESTINATION "include/symbol" # target directory
        FILES_MATCHING # install only matched files
        PATTERN "*.h" # select header files
)
