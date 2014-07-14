#ifndef sdl_keycode_conversion_H
#define sdl_keycode_conversion_H

#include "gnat.h"
#include "platform/keycodes.h"
#include "SDL.h"

namespace gnat {

KeyCode sdl_keysym_to_keycode(SDL_Keycode k);

} // namespace gnat

#endif // sdl_keycode_conversion_H
