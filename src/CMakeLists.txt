set(GNAT_FILES
  gnat.h

  event/event_handler.h
  event/message.h
  event/observable.cc
  event/observable.h
  event/observer.cc
  event/observer.h
  event/signal.cc
  event/signal.h
  event/slot.cc
  event/slot.h

  gfx/graphics_context.cc
  gfx/graphics_context.h
  gfx/math/vector3.cc
  gfx/math/vector3.h

  platform/gl_platform_context.h
  platform/input_platform_context.h
  platform/platform_context.h

  platform/sdl/sdl_platform_context.cc
  platform/sdl/sdl_platform_context.h

  third_party/FastDelegate/FastDelegate.h

  util/color.h
  util/logger.cc
  util/logger.h
  util/string_utils.cc
  util/string_utils.h
  util/timer.cc
  util/timer.h
	)

add_library(gnat SHARED ${GNAT_FILES} )
set_target_properties(gnat PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(gnat X11 GL SDL2)

install(TARGETS gnat
	RUNTIME DESTINATION ${INSTALL_RUNTIMES_TO}
	LIBRARY DESTINATION ${INSTALL_LIBS_TO}
	ARCHIVE DESTINATION ${INSTALL_LIBS_TO})

if (WIN32)
	set(LIB_PREFIX )
	set(LIB_POSTFIX .lib)
else ()
	set(LIB_PREFIX lib)
	set(LIB_POSTFIX .so)
endif ()

include_directories("src")

set(TEST_FILES
  event/observer_unittest.cc
  event/observable_unittest.cc
  event/signal_unittest.cc
  event/slot_unittest.cc

  gfx/math/vector3_unittest.cc

  tests/test_main.cc

  util/color_unittest.cc
  util/string_utils_unittest.cc
	)

add_executable(gnat_tests ${TEST_FILES})
set_target_properties(gnat_tests PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries (gnat_tests gtest pthread gnat)

set(PERFTEST_FILES
  tests/test_main.cc
  
  gfx/math/vector3_perftest.cc
	)

add_executable(gnat_perftests ${PERFTEST_FILES})
set_target_properties(gnat_perftests PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries (gnat_perftests gtest pthread gnat)
