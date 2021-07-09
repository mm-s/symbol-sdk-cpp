#====================================== symbol-offline
add_executable(symbol-offline main_core.cpp)
target_include_directories(symbol-offline PUBLIC
	# Headers used from source/build location:
	"$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>"
	"$<BUILD_INTERFACE:${PROJECT_BINARY_DIR}>"
	# Headers used from installed location:
	"$<INSTALL_INTERFACE:include>"
)
target_link_libraries(symbol-offline PUBLIC symbolcore)
install(TARGETS symbol-offline DESTINATION bin)

#====================================== symbol (online-cli)
if (NOT ${ONLY_SYMBOLCORE})
	add_executable(symbol main.cpp)
	target_include_directories(symbol PUBLIC
		# Headers used from source/build location:
		"$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>"
		"$<BUILD_INTERFACE:${PROJECT_BINARY_DIR}>"
		# Headers used from installed location:
		"$<INSTALL_INTERFACE:include>"
	)
	target_link_libraries(symbol PUBLIC symbolcore)
	target_link_libraries(symbol PUBLIC symbolrpc)
	install(TARGETS symbol DESTINATION bin)
endif()

