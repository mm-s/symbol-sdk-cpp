add_executable(symbol main.cpp)

target_include_directories(symbol
  INTERFACE
    # Headers used from source/build location:
    "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>"
    # Headers used from installed location:
    "$<INSTALL_INTERFACE:include>"
)

target_link_libraries(symbol PUBLIC symbolrpc)

install(TARGETS symbol DESTINATION bin)

