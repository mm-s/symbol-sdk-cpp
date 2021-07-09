include_directories(SYSTEM ${GTEST_INCLUDE_DIR})

file(GLOB test_SRC "*.cpp")
file(GLOB hmi_SRC "Hmi/*.cpp")

add_executable(symbol-offline-test
	${test_SRC}
	${hmi_SRC}
)
target_include_directories(symbol-offline-test INTERFACE
	# Headers used from source/build location:
	"$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>"
	# Headers used from installed location:
	"$<INSTALL_INTERFACE:include>"
)
target_link_libraries(symbol-offline-test PUBLIC symbolcore gtest gtest_main)
add_test( runUnitTests runUnitTests )

