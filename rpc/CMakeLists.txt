configure_file(config.h.in config.h)

file(GLOB symbol_SRC *.cpp)
file(GLOB symbol_hmi_SRC "Hmi/*.cpp")
file(GLOB symbol_hmi_transaction_SRC "Hmi/Transaction/*.cpp")

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

#------------- libsymbolrpc.so
add_library(symbolrpc SHARED
	${symbol_SRC}
	${symbol_hmi_SRC}
	${symbol_hmi_transaction_SRC}
)

add_compile_definitions("BOOST_ALL_DYN_LINK")
include_directories(${Boost_INCLUDE_DIRS})
target_link_libraries(symbolrpc ${Boost_LIBRARIES})

include_directories(${RESTCCPP_INCLUDE_DIRS})
target_link_libraries(symbolrpc ${RESTCCPP_LIBRARIES})
target_link_libraries(symbolrpc symbolcore)
target_link_libraries(symbolrpc OpenSSL::SSL)
target_link_libraries(symbolrpc ZLIB::ZLIB)
target_link_libraries(symbolrpc ${JSONCPP_LIBRARIES})

set_property(TARGET symbolrpc PROPERTY POSITION_INDEPENDENT_CODE ON)

target_include_directories(symbolrpc PUBLIC
	# Headers used from source/build location:
	"$<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/rpc>"
	"$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>"
	# Headers used from installed location:
	"$<INSTALL_INTERFACE:include>"
)
install(TARGETS symbolrpc DESTINATION lib)

install(DIRECTORY "${CMAKE_SOURCE_DIR}/rpc/" # source directory
	DESTINATION "include/symbol/rpc" # target directory
	FILES_MATCHING # install only matched files
	PATTERN "*.h" # select header files
)

install(FILES "${CMAKE_SOURCE_DIR}/cmake/Modules/FindSYMBOLRPC.cmake" DESTINATION "share/cmake/SYMBOLRPC")
install(FILES "${CMAKE_SOURCE_DIR}/include/symbol/rpc.h" DESTINATION "include/symbol/")
install(FILES "${CMAKE_BINARY_DIR}/rpc/config.h" DESTINATION "include/symbol/rpc/")

