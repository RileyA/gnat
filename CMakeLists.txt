cmake_minimum_required (VERSION 2.6)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin")
set(INSTALL_RUNTIMES_TO "/usr/bin" CACHE FILEPATH "Where install executables.")
set(INSTALL_LIBS_TO "/usr/lib" CACHE FILEPATH "Where to install libs.")

include_directories(src)
add_subdirectory(src)
