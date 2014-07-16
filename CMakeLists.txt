cmake_minimum_required (VERSION 2.6)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin")
set(INSTALL_RUNTIMES_TO "/usr/bin" CACHE FILEPATH "Where install executables.")
set(INSTALL_LIBS_TO "/usr/lib" CACHE FILEPATH "Where to install libs.")

if(CMAKE_BUILD_TYPE MATCHES "[Dd][Ee][Bb][Uu][Gg]")
  set(DEBUG_BUILD 1)
  add_definitions(-DDEBUG)
endif()

enable_language(ASM_NASM)
set(CMAKE_ASM_NASM_COMPILER "yasm")

include_directories(src)
include_directories(/usr/local/include/SDL2)
include_directories(src/third_party/gtest/include)
include_directories(/usr/include/GL)
include_directories(/usr/include)
include_directories(/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.9.sdk/System/Library/Frameworks/OpenGL.framework/Versions/A/Headers/)
link_directories(/usr/local/lib;/usr/lib)
add_subdirectory(src)
add_subdirectory(samples)
