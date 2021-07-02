include_directories(SYSTEM ${GTEST_INCLUDE_DIR})
add_executable(symbol-offline-test main.cpp)
target_include_directories(symbol-offline-test INTERFACE
	# Headers used from source/build location:
	"$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>"
	# Headers used from installed location:
	"$<INSTALL_INTERFACE:include>"
)
target_link_libraries(symbol-offline-test PUBLIC symbolcore)

