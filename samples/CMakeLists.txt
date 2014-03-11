cmake_minimum_required (VERSION 2.6)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin")
set(SAMPLE_FILES
  sample.cc
	)

include_directories(../src)
cmake_policy(SET CMP0015 NEW)
link_directories(../lib)

add_executable(sample ${SAMPLE_FILES} )
set_target_properties(sample PROPERTIES LINKER_LANGUAGE CXX)
link_directories(/usr/local/lib)

target_link_libraries(sample gnat SDL2)
