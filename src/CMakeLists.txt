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
  gfx/gl_platform_context.h
  gfx/platform/glx_platform_context.cc
  event/slot.h
  third_party/FastDelegate/FastDelegate.h
  util/logger.cc
  util/logger.h
	)

add_library(gnat SHARED ${GNAT_FILES} )
set_target_properties(gnat PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(gnat X11 GL)

install(TARGETS gnat
	RUNTIME DESTINATION ${INSTALL_RUNTIMES_TO}
	LIBRARY DESTINATION ${INSTALL_LIBS_TO}
	ARCHIVE DESTINATION ${INSTALL_LIBS_TO})
