cmake_minimum_required (VERSION 2.6)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin")
set(INSTALL_RUNTIMES_TO "/usr/bin" CACHE FILEPATH "Where install executables.")
set(INSTALL_LIBS_TO "/usr/lib" CACHE FILEPATH "Where to install libs.")

if(CMAKE_BUILD_TYPE MATCHES "[Dd][Ee][Bb][Uu][Gg]")
  set(DEBUG_BUILD 1)
  add_definitions(-DDEBUG)
endif()

include_directories(src)
include_directories(/usr/local/include/SDL2)
include_directories(/usr/include/GL)
link_directories(/usr/local/lib)
add_subdirectory(src)
add_subdirectory(samples)
