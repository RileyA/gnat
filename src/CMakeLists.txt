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

  game/game.cc
  game/game.h
  game/game_state.h

  gfx/framebuffer_object.cc
  gfx/framebuffer_object.h
  gfx/material/material.cc
  gfx/material/material.h
  gfx/material/program.cc
  gfx/material/program.h
  gfx/material/shader.cc
  gfx/material/shader.h
  gfx/material/texture.cc
  gfx/material/texture.h
  gfx/graphics_context.cc
  gfx/graphics_context.h
  gfx/scene/camera.cc
  gfx/scene/camera.h
  gfx/scene/drawable.cc
  gfx/scene/drawable.h
  gfx/scene/fps_camera.cc
  gfx/scene/fps_camera.h
  gfx/scene/mesh.cc
  gfx/scene/mesh.h
  gfx/scene/mesh_drawable.cc
  gfx/scene/mesh_drawable.h
  gfx/scene/node.cc
  gfx/scene/node.h
  gfx/scene/scene_manager.cc
  gfx/scene/scene_manager.h
  gfx/util/buffer_object.cc
  gfx/util/buffer_object.h
  gfx/util/mesh_data.cc
  gfx/util/mesh_data.h
  gfx/util/oyster_mesh.cc
  gfx/util/oyster_mesh.h
  gfx/math/quaternion.cc
  gfx/math/quaternion.h
  gfx/math/vector3.cc
  gfx/math/vector3.h
  gfx/math/vector4.cc
  gfx/math/vector4.h
  gfx/math/matrix3.cc
  gfx/math/matrix3.h
  gfx/math/matrix4.cc
  gfx/math/matrix4.h

  platform/gl_platform_context.h
  platform/input_platform_context.h
  platform/keycodes.cc
  platform/keycodes.h
  platform/platform_context.h

  platform/sdl/sdl_platform_context.cc
  platform/sdl/sdl_platform_context.h

  third_party/FastDelegate/FastDelegate.h

  util/color.h
  util/file_utils.cc
  util/file_utils.h
  util/json.cc
  util/json.h
  util/logger.cc
  util/logger.h
  util/scoped_ptr.h
  util/string_utils.cc
  util/string_utils.h
  util/timer.cc
  util/timer.h
	)

find_package(OpenGL)

add_library(gnat SHARED ${GNAT_FILES} )
set_target_properties(gnat PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(gnat SDL2 GLEW png ${OPENGL_LIBRARIES})

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
include_directories("src/third_party/glew/include")

set(TEST_FILES
  event/observer_unittest.cc
  event/observable_unittest.cc
  event/signal_unittest.cc
  event/slot_unittest.cc

  gfx/math/vector3_unittest.cc
  gfx/scene/node_unittest.cc
  gfx/scene/drawable_unittest.cc

  tests/test_main.cc

  util/color_unittest.cc
  util/file_utils_unittest.cc
  util/json_unittest.cc
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
