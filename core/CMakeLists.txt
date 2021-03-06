file(GLOB symbol_core_SRC *.cpp)
file(GLOB symbol_core_catapult_SRC "catapult/*.cpp")
file(GLOB symbol_core_hmi_SRC "Hmi/*.cpp")
file(GLOB symbol_core_hmi_wallet_SRC "Hmi/Wallet/*.cpp")
file(GLOB symbol_core_transaction_SRC "Transaction/*.cpp")
file(GLOB symbol_core_hmi_transaction_SRC "Hmi/Transaction/*.cpp")
file(GLOB symbol_core_donna_SRC "donna/*.c")
file(GLOB symbol_dto_SRC "dto/*.cpp")

add_definitions(-DNO_MISALIGNED_ACCESSES) # sha3

#------------- libsymbolcore.so
add_library(symbolcore SHARED
	${symbol_core_SRC}
	${symbol_core_catapult_SRC}
	${symbol_core_donna_SRC}
	${symbol_core_hmi_SRC}
	${symbol_core_hmi_wallet_SRC}
	${symbol_core_transaction_SRC}
	${symbol_core_hmi_transaction_SRC}
	${symbol_dto_SRC}
)

#-------------Use OpenSSL, Boost
include_directories(${OPENSSL_INCLUDE_DIR})
target_link_libraries(symbolcore OpenSSL::SSL)

add_compile_definitions("BOOST_ALL_DYN_LINK")
include_directories(${Boost_INCLUDE_DIRS})
target_link_libraries(symbolcore ${Boost_LIBRARIES})

target_link_libraries(symbolcore ${JSONCPP_LIBRARIES})

set_property(TARGET symbolcore PROPERTY POSITION_INDEPENDENT_CODE ON)

target_include_directories(symbolcore PUBLIC
	# Headers used from source/build location:
	"$<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/core>"
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

install(FILES "${CMAKE_SOURCE_DIR}/cmake/Modules/FindSYMBOLCORE.cmake" DESTINATION "share/cmake/SYMBOLCORE/")
install(FILES "${CMAKE_SOURCE_DIR}/include/symbol/core.h" DESTINATION "include/symbol/")


