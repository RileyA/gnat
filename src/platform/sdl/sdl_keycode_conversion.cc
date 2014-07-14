#include "platform/sdl/sdl_keycode_conversion.h"

namespace gnat {

KeyCode sdl_keysym_to_keycode(SDL_Keycode k) {
  switch (k) {
    case SDLK_0:
      return K_0;
      break;
    case SDLK_1:
      return K_1;
      break;
    case SDLK_2:
      return K_2;
      break;
    case SDLK_3:
      return K_3;
      break;
    case SDLK_4:
      return K_4;
      break;
    case SDLK_5:
      return K_5;
      break;
    case SDLK_6:
      return K_6;
      break;
    case SDLK_7:
      return K_7;
      break;
    case SDLK_8:
      return K_8;
      break;
    case SDLK_9:
      return K_9;
      break;
    case SDLK_a:
      return K_A;
      break;
    case SDLK_AC_BACK:
      return K_AC_BACK;
      break;
    case SDLK_AC_BOOKMARKS:
      return K_AC_BOOKMARKS;
      break;
    case SDLK_AC_FORWARD:
      return K_AC_FORWARD;
      break;
    case SDLK_AC_HOME:
      return K_AC_HOME;
      break;
    case SDLK_AC_REFRESH:
      return K_AC_REFRESH;
      break;
    case SDLK_AC_SEARCH:
      return K_AC_SEARCH;
      break;
    case SDLK_AC_STOP:
      return K_AC_STOP;
      break;
    case SDLK_AGAIN:
      return K_AGAIN;
      break;
    case SDLK_ALTERASE:
      return K_ALTERASE;
      break;
    case SDLK_QUOTE:
      return K_QUOTE;
      break;
    case SDLK_APPLICATION:
      return K_APPLICATION;
      break;
    case SDLK_AUDIOMUTE:
      return K_AUDIOMUTE;
      break;
    case SDLK_AUDIONEXT:
      return K_AUDIONEXT;
      break;
    case SDLK_AUDIOPLAY:
      return K_AUDIOPLAY;
      break;
    case SDLK_AUDIOPREV:
      return K_AUDIOPREV;
      break;
    case SDLK_AUDIOSTOP:
      return K_AUDIOSTOP;
      break;
    case SDLK_b:
      return K_B;
      break;
    case SDLK_BACKSLASH:
      return K_BACKSLASH;
      break;
    case SDLK_BACKSPACE:
      return K_BACKSPACE;
      break;
    case SDLK_BRIGHTNESSDOWN:
      return K_BRIGHTNESSDOWN;
      break;
    case SDLK_BRIGHTNESSUP:
      return K_BRIGHTNESSUP;
      break;
    case SDLK_c:
      return K_C;
      break;
    case SDLK_CALCULATOR:
      return K_CALCULATOR;
      break;
    case SDLK_CANCEL:
      return K_CANCEL;
      break;
    case SDLK_CAPSLOCK:
      return K_CAPSLOCK;
      break;
    case SDLK_CLEAR:
      return K_CLEAR;
      break;
    case SDLK_CLEARAGAIN:
      return K_CLEARAGAIN;
      break;
    case SDLK_COMMA:
      return K_COMMA;
      break;
    case SDLK_COMPUTER:
      return K_COMPUTER;
      break;
    case SDLK_COPY:
      return K_COPY;
      break;
    case SDLK_CRSEL:
      return K_CRSEL;
      break;
    case SDLK_CURRENCYSUBUNIT:
      return K_CURRENCYSUBUNIT;
      break;
    case SDLK_CURRENCYUNIT:
      return K_CURRENCYUNIT;
      break;
    case SDLK_CUT:
      return K_CUT;
      break;
    case SDLK_d:
      return K_D;
      break;
    case SDLK_DECIMALSEPARATOR:
      return K_DECIMALSEPARATOR;
      break;
    case SDLK_DELETE:
      return K_DELETE;
      break;
    case SDLK_DISPLAYSWITCH:
      return K_DISPLAYSWITCH;
      break;
    case SDLK_DOWN:
      return K_DOWN;
      break;
    case SDLK_e:
      return K_E;
      break;
    case SDLK_EJECT:
      return K_EJECT;
      break;
    case SDLK_END:
      return K_END;
      break;
    case SDLK_EQUALS:
      return K_EQUALS;
      break;
    case SDLK_ESCAPE:
      return K_ESCAPE;
      break;
    case SDLK_EXECUTE:
      return K_EXECUTE;
      break;
    case SDLK_EXSEL:
      return K_EXSEL;
      break;
    case SDLK_f:
      return K_F;
      break;
    case SDLK_F1:
      return K_F1;
      break;
    case SDLK_F10:
      return K_F10;
      break;
    case SDLK_F11:
      return K_F11;
      break;
    case SDLK_F12:
      return K_F12;
      break;
    case SDLK_F13:
      return K_F13;
      break;
    case SDLK_F14:
      return K_F14;
      break;
    case SDLK_F15:
      return K_F15;
      break;
    case SDLK_F16:
      return K_F16;
      break;
    case SDLK_F17:
      return K_F17;
      break;
    case SDLK_F18:
      return K_F18;
      break;
    case SDLK_F19:
      return K_F19;
      break;
    case SDLK_F2:
      return K_F2;
      break;
    case SDLK_F20:
      return K_F20;
      break;
    case SDLK_F21:
      return K_F21;
      break;
    case SDLK_F22:
      return K_F22;
      break;
    case SDLK_F23:
      return K_F23;
      break;
    case SDLK_F24:
      return K_F24;
      break;
    case SDLK_F3:
      return K_F3;
      break;
    case SDLK_F4:
      return K_F4;
      break;
    case SDLK_F5:
      return K_F5;
      break;
    case SDLK_F6:
      return K_F6;
      break;
    case SDLK_F7:
      return K_F7;
      break;
    case SDLK_F8:
      return K_F8;
      break;
    case SDLK_F9:
      return K_F9;
      break;
    case SDLK_FIND:
      return K_FIND;
      break;
    case SDLK_g:
      return K_G;
      break;
    case SDLK_BACKQUOTE:
      return K_BACKQUOTE;
      break;
    case SDLK_h:
      return K_H;
      break;
    case SDLK_HELP:
      return K_HELP;
      break;
    case SDLK_HOME:
      return K_HOME;
      break;
    case SDLK_i:
      return K_I;
      break;
    case SDLK_INSERT:
      return K_INSERT;
      break;
    case SDLK_j:
      return K_J;
      break;
    case SDLK_k:
      return K_K;
      break;
    case SDLK_KBDILLUMDOWN:
      return K_KBDILLUMDOWN;
      break;
    case SDLK_KBDILLUMTOGGLE:
      return K_KBDILLUMTOGGLE;
      break;
    case SDLK_KBDILLUMUP:
      return K_KBDILLUMUP;
      break;
    case SDLK_KP_0:
      return K_KP_0;
      break;
    case SDLK_KP_00:
      return K_KP_00;
      break;
    case SDLK_KP_000:
      return K_KP_000;
      break;
    case SDLK_KP_1:
      return K_KP_1;
      break;
    case SDLK_KP_2:
      return K_KP_2;
      break;
    case SDLK_KP_3:
      return K_KP_3;
      break;
    case SDLK_KP_4:
      return K_KP_4;
      break;
    case SDLK_KP_5:
      return K_KP_5;
      break;
    case SDLK_KP_6:
      return K_KP_6;
      break;
    case SDLK_KP_7:
      return K_KP_7;
      break;
    case SDLK_KP_8:
      return K_KP_8;
      break;
    case SDLK_KP_9:
      return K_KP_9;
      break;
    case SDLK_KP_A:
      return K_KP_A;
      break;
    case SDLK_KP_AMPERSAND:
      return K_KP_AMPERSAND;
      break;
    case SDLK_KP_AT:
      return K_KP_AT;
      break;
    case SDLK_KP_B:
      return K_KP_B;
      break;
    case SDLK_KP_BACKSPACE:
      return K_KP_BACKSPACE;
      break;
    case SDLK_KP_BINARY:
      return K_KP_BINARY;
      break;
    case SDLK_KP_C:
      return K_KP_C;
      break;
    case SDLK_KP_CLEAR:
      return K_KP_CLEAR;
      break;
    case SDLK_KP_CLEARENTRY:
      return K_KP_CLEARENTRY;
      break;
    case SDLK_KP_COLON:
      return K_KP_COLON;
      break;
    case SDLK_KP_COMMA:
      return K_KP_COMMA;
      break;
    case SDLK_KP_D:
      return K_KP_D;
      break;
    case SDLK_KP_DBLAMPERSAND:
      return K_KP_DBLAMPERSAND;
      break;
    case SDLK_KP_DBLVERTICALBAR:
      return K_KP_DBLVERTICALBAR;
      break;
    case SDLK_KP_DECIMAL:
      return K_KP_DECIMAL;
      break;
    case SDLK_KP_DIVIDE:
      return K_KP_DIVIDE;
      break;
    case SDLK_KP_E:
      return K_KP_E;
      break;
    case SDLK_KP_ENTER:
      return K_KP_ENTER;
      break;
    case SDLK_KP_EQUALS:
      return K_KP_EQUALS;
      break;
    case SDLK_KP_EQUALSAS400:
      return K_KP_EQUALSAS400;
      break;
    case SDLK_KP_EXCLAM:
      return K_KP_EXCLAM;
      break;
    case SDLK_KP_F:
      return K_KP_F;
      break;
    case SDLK_KP_GREATER:
      return K_KP_GREATER;
      break;
    case SDLK_KP_HASH:
      return K_KP_HASH;
      break;
    case SDLK_KP_HEXADECIMAL:
      return K_KP_HEXADECIMAL;
      break;
    case SDLK_KP_LEFTBRACE:
      return K_KP_LEFTBRACE;
      break;
    case SDLK_KP_LEFTPAREN:
      return K_KP_LEFTPAREN;
      break;
    case SDLK_KP_LESS:
      return K_KP_LESS;
      break;
    case SDLK_KP_MEMADD:
      return K_KP_MEMADD;
      break;
    case SDLK_KP_MEMCLEAR:
      return K_KP_MEMCLEAR;
      break;
    case SDLK_KP_MEMDIVIDE:
      return K_KP_MEMDIVIDE;
      break;
    case SDLK_KP_MEMMULTIPLY:
      return K_KP_MEMMULTIPLY;
      break;
    case SDLK_KP_MEMRECALL:
      return K_KP_MEMRECALL;
      break;
    case SDLK_KP_MEMSTORE:
      return K_KP_MEMSTORE;
      break;
    case SDLK_KP_MEMSUBTRACT:
      return K_KP_MEMSUBTRACT;
      break;
    case SDLK_KP_MINUS:
      return K_KP_MINUS;
      break;
    case SDLK_KP_MULTIPLY:
      return K_KP_MULTIPLY;
      break;
    case SDLK_KP_OCTAL:
      return K_KP_OCTAL;
      break;
    case SDLK_KP_PERCENT:
      return K_KP_PERCENT;
      break;
    case SDLK_KP_PERIOD:
      return K_KP_PERIOD;
      break;
    case SDLK_KP_PLUS:
      return K_KP_PLUS;
      break;
    case SDLK_KP_PLUSMINUS:
      return K_KP_PLUSMINUS;
      break;
    case SDLK_KP_POWER:
      return K_KP_POWER;
      break;
    case SDLK_KP_RIGHTBRACE:
      return K_KP_RIGHTBRACE;
      break;
    case SDLK_KP_RIGHTPAREN:
      return K_KP_RIGHTPAREN;
      break;
    case SDLK_KP_SPACE:
      return K_KP_SPACE;
      break;
    case SDLK_KP_TAB:
      return K_KP_TAB;
      break;
    case SDLK_KP_VERTICALBAR:
      return K_KP_VERTICALBAR;
      break;
    case SDLK_KP_XOR:
      return K_KP_XOR;
      break;
    case SDLK_l:
      return K_L;
      break;
    case SDLK_LALT:
      return K_LALT;
      break;
    case SDLK_LCTRL:
      return K_LCTRL;
      break;
    case SDLK_LEFT:
      return K_LEFT;
      break;
    case SDLK_LEFTBRACKET:
      return K_LEFTBRACKET;
      break;
    case SDLK_LGUI:
      return K_LGUI;
      break;
    case SDLK_LSHIFT:
      return K_LSHIFT;
      break;
    case SDLK_m:
      return K_M;
      break;
    case SDLK_MAIL:
      return K_MAIL;
      break;
    case SDLK_MEDIASELECT:
      return K_MEDIASELECT;
      break;
    case SDLK_MENU:
      return K_MENU;
      break;
    case SDLK_MINUS:
      return K_MINUS;
      break;
    case SDLK_MODE:
      return K_MODE;
      break;
    case SDLK_MUTE:
      return K_MUTE;
      break;
    case SDLK_n:
      return K_N;
      break;
    case SDLK_NUMLOCKCLEAR:
      return K_NUMLOCKCLEAR;
      break;
    case SDLK_o:
      return K_O;
      break;
    case SDLK_OPER:
      return K_OPER;
      break;
    case SDLK_OUT:
      return K_OUT;
      break;
    case SDLK_p:
      return K_P;
      break;
    case SDLK_PAGEDOWN:
      return K_PAGEDOWN;
      break;
    case SDLK_PAGEUP:
      return K_PAGEUP;
      break;
    case SDLK_PASTE:
      return K_PASTE;
      break;
    case SDLK_PAUSE:
      return K_PAUSE;
      break;
    case SDLK_PERIOD:
      return K_PERIOD;
      break;
    case SDLK_POWER:
      return K_POWER;
      break;
    case SDLK_PRINTSCREEN:
      return K_PRINTSCREEN;
      break;
    case SDLK_PRIOR:
      return K_PRIOR;
      break;
    case SDLK_q:
      return K_Q;
      break;
    case SDLK_r:
      return K_R;
      break;
    case SDLK_RALT:
      return K_RALT;
      break;
    case SDLK_RCTRL:
      return K_RCTRL;
      break;
    case SDLK_RETURN:
      return K_RETURN;
      break;
    case SDLK_RETURN2:
      return K_RETURN2;
      break;
    case SDLK_RGUI:
      return K_RGUI;
      break;
    case SDLK_RIGHT:
      return K_RIGHT;
      break;
    case SDLK_RIGHTBRACKET:
      return K_RIGHTBRACKET;
      break;
    case SDLK_RSHIFT:
      return K_RSHIFT;
      break;
    case SDLK_s:
      return K_S;
      break;
    case SDLK_SCROLLLOCK:
      return K_SCROLLLOCK;
      break;
    case SDLK_SELECT:
      return K_SELECT;
      break;
    case SDLK_SEMICOLON:
      return K_SEMICOLON;
      break;
    case SDLK_SEPARATOR:
      return K_SEPARATOR;
      break;
    case SDLK_SLASH:
      return K_SLASH;
      break;
    case SDLK_SLEEP:
      return K_SLEEP;
      break;
    case SDLK_SPACE:
      return K_SPACE;
      break;
    case SDLK_STOP:
      return K_STOP;
      break;
    case SDLK_SYSREQ:
      return K_SYSREQ;
      break;
    case SDLK_t:
      return K_T;
      break;
    case SDLK_TAB:
      return K_TAB;
      break;
    case SDLK_THOUSANDSSEPARATOR:
      return K_THOUSANDSSEPARATOR;
      break;
    case SDLK_u:
      return K_U;
      break;
    case SDLK_UNDO:
      return K_UNDO;
      break;
    case SDLK_UNKNOWN:
      return K_UNKNOWN;
      break;
    case SDLK_UP:
      return K_UP;
      break;
    case SDLK_v:
      return K_V;
      break;
    case SDLK_VOLUMEDOWN:
      return K_VOLUMEDOWN;
      break;
    case SDLK_VOLUMEUP:
      return K_VOLUMEUP;
      break;
    case SDLK_w:
      return K_W;
      break;
    case SDLK_WWW:
      return K_WWW;
      break;
    case SDLK_x:
      return K_X;
      break;
    case SDLK_y:
      return K_Y;
      break;
    case SDLK_z:
      return K_Z;
      break;
    case SDLK_AMPERSAND:
      return K_AMPERSAND;
      break;
    case SDLK_ASTERISK:
      return K_ASTERISK;
      break;
    case SDLK_AT:
      return K_AT;
      break;
    case SDLK_CARET:
      return K_CARET;
      break;
    case SDLK_COLON:
      return K_COLON;
      break;
    case SDLK_DOLLAR:
      return K_DOLLAR;
      break;
    case SDLK_EXCLAIM:
      return K_EXCLAIM;
      break;
    case SDLK_GREATER:
      return K_GREATER;
      break;
    case SDLK_HASH:
      return K_HASH;
      break;
    case SDLK_LEFTPAREN:
      return K_LEFTPAREN;
      break;
    case SDLK_LESS:
      return K_LESS;
      break;
    case SDLK_PERCENT:
      return K_PERCENT;
      break;
    case SDLK_PLUS:
      return K_PLUS;
      break;
    case SDLK_QUESTION:
      return K_QUESTION;
      break;
    case SDLK_QUOTEDBL:
      return K_QUOTEDBL;
      break;
    case SDLK_RIGHTPAREN:
      return K_RIGHTPAREN;
      break;
    case SDLK_UNDERSCORE:
      return K_UNDERSCORE;
      break;
    default:
      NOTREACHED();
      return K_0;
  }
}

}  // namespace gnat
