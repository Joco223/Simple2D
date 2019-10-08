#pragma once

#include <SDL.h>
#include <math.h>
#include <algorithm>
#include <iostream>
#include <memory>

namespace Simple2D {
	struct Colour {
		unsigned char red;
		unsigned char green;
		unsigned char blue;
		unsigned char alpha;
	};

	enum blending_modes {
		S2D_BLENDING_NONE = 0,
		S2D_BLENDING_ALPHA,
		S2D_BLENDING_ADD,
		S2D_BLENDING_MOD
	};

	enum antialisaing_modes {
		S2D_AA_NEAREST = 0,
		S2D_AA_LINEAR,
		S2D_AA_ANISOTROPIC
	};
	//Event structs

	struct keyboard_e {
		unsigned char state;
		int key_code;
		unsigned short mod;
		unsigned char character;
	};

	//States
	constexpr unsigned char KEY_DOWN = SDL_PRESSED;
	constexpr unsigned char KEY_UP   = SDL_RELEASED;

	//Key codes
	constexpr int KEY_ZERO        = SDLK_0;
	constexpr int KEY_ONE         = SDLK_1;
	constexpr int KEY_TWO         = SDLK_2;
	constexpr int KEY_THREE       = SDLK_3;
	constexpr int KEY_FOUR        = SDLK_4;
	constexpr int KEY_FIVE        = SDLK_5;
	constexpr int KEY_SIX         = SDLK_6;
	constexpr int KEY_SEVEN       = SDLK_7;
	constexpr int KEY_EIGHT       = SDLK_8;
	constexpr int KEY_NINE        = SDLK_9;

	constexpr int KEY_KP_ZERO     = SDLK_KP_0;
	constexpr int KEY_KP_ONE      = SDLK_KP_1;
	constexpr int KEY_KP_TWO      = SDLK_KP_2;
	constexpr int KEY_KP_THREE    = SDLK_KP_3;
	constexpr int KEY_KP_FOUR     = SDLK_KP_4;
	constexpr int KEY_KP_FIVE     = SDLK_KP_5;
	constexpr int KEY_KP_SIX      = SDLK_KP_6;
	constexpr int KEY_KP_SEVEN    = SDLK_KP_7;
	constexpr int KEY_KP_EIGHT    = SDLK_KP_8;
	constexpr int KEY_KP_NINE     = SDLK_KP_9;
	constexpr int KEY_KP_PERIOD   = SDLK_KP_PERIOD;
	constexpr int KEY_KP_DIVIDIE  = SDLK_KP_DIVIDE;
	constexpr int KEY_KP_MULTIPLY = SDLK_KP_MULTIPLY;
	constexpr int KEY_KP_ENTER    = SDLK_KP_ENTER;
	constexpr int KEY_KP_MINUS    = SDLK_KP_MINUS;
	constexpr int KEY_KP_PLUS     = SDLK_KP_PLUS;

	constexpr int KEY_A           = SDLK_a;
	constexpr int KEY_B           = SDLK_b;
	constexpr int KEY_C           = SDLK_c;
	constexpr int KEY_D           = SDLK_d;
	constexpr int KEY_E           = SDLK_e;
	constexpr int KEY_F           = SDLK_f;
	constexpr int KEY_G           = SDLK_g;
	constexpr int KEY_H           = SDLK_h;
	constexpr int KEY_I           = SDLK_i;
	constexpr int KEY_J           = SDLK_j;
	constexpr int KEY_K           = SDLK_k;
	constexpr int KEY_L           = SDLK_l;
	constexpr int KEY_M           = SDLK_m;
	constexpr int KEY_N           = SDLK_n;
	constexpr int KEY_O           = SDLK_o;
	constexpr int KEY_P           = SDLK_p;
	constexpr int KEY_Q           = SDLK_q;
	constexpr int KEY_R           = SDLK_r;
	constexpr int KEY_S           = SDLK_s;
	constexpr int KEY_T           = SDLK_t;
	constexpr int KEY_U           = SDLK_u;
	constexpr int KEY_V           = SDLK_v;
	constexpr int KEY_W           = SDLK_w;
	constexpr int KEY_X           = SDLK_x;
	constexpr int KEY_Y           = SDLK_y;
	constexpr int KEY_Z           = SDLK_z;

	constexpr int KEY_F1          = SDLK_F1;
	constexpr int KEY_F2          = SDLK_F2;
	constexpr int KEY_F3          = SDLK_F3;
	constexpr int KEY_F4          = SDLK_F4;
	constexpr int KEY_F5          = SDLK_F5;
	constexpr int KEY_F6          = SDLK_F6;
	constexpr int KEY_F7          = SDLK_F7;
	constexpr int KEY_F8          = SDLK_F8;
	constexpr int KEY_F9          = SDLK_F9;
	constexpr int KEY_F10         = SDLK_F10;
	constexpr int KEY_F11         = SDLK_F11;
	constexpr int KEY_F12         = SDLK_F12;

	constexpr int KEY_A_DOWN      = SDLK_DOWN;
	constexpr int KEY_A_LEFT      = SDLK_LEFT;
	constexpr int KEY_A_RIGHT     = SDLK_RIGHT;
	constexpr int KEY_A_UP        = SDLK_UP;

	constexpr int KEY_QUOTE       = SDLK_QUOTE;
	constexpr int KEY_BACKSLASH   = SDLK_BACKSLASH;
	constexpr int KEY_BACKSPACE   = SDLK_BACKSPACE;
	constexpr int KEY_CAPSLOCK    = SDLK_CAPSLOCK;
	constexpr int KEY_COMMA       = SDLK_COMMA;
	constexpr int KEY_DELETE      = SDLK_DELETE;
	constexpr int KEY_END         = SDLK_END;
	constexpr int KEY_EQUALS      = SDLK_EQUALS;
	constexpr int KEY_ESC         = SDLK_ESCAPE;
	constexpr int KEY_GRAVE       = SDLK_BACKQUOTE;
	constexpr int KEY_HOME        = SDLK_HOME;
	constexpr int KEY_INSERT      = SDLK_INSERT;
	constexpr int KEY_L_ALT       = SDLK_LALT;
	constexpr int KEY_R_ALT       = SDLK_RALT;
	constexpr int KEY_L_CTRL      = SDLK_LCTRL;
	constexpr int KEY_R_CTRL      = SDLK_RCTRL;
	constexpr int KEY_L_SHIFT     = SDLK_LSHIFT;
	constexpr int KEY_R_SHIFT     = SDLK_RSHIFT;
	constexpr int KEY_L_BRACKET   = SDLK_LEFTBRACKET;
	constexpr int KEY_R_BRACKET   = SDLK_RIGHTBRACKET;
	constexpr int KEY_MINUS       = SDLK_MINUS;
	constexpr int KEY_NUMLOCK     = SDLK_NUMLOCKCLEAR;
	constexpr int KEY_PAGE_UP     = SDLK_PAGEUP;
	constexpr int KEY_PAGE_DOWN   = SDLK_PAGEDOWN;
	constexpr int KEY_PERIOD      = SDLK_PERIOD;
	constexpr int KEY_PRINTSCREEN = SDLK_PRINTSCREEN;
	constexpr int KEY_RETURN      = SDLK_RETURN;
	constexpr int KEY_SCROLLLOCK  = SDLK_SCROLLLOCK;
	constexpr int KEY_SEMICOLON   = SDLK_SEMICOLON;
	constexpr int KEY_SLASH       = SDLK_SLASH;
	constexpr int KEY_SOACE       = SDLK_SPACE;
	constexpr int KEY_TAB         = SDLK_TAB;

	//Mod flags
	constexpr unsigned short KEYMOD_NONE     = KMOD_NONE;
	constexpr unsigned short KEYMOD_L_SHIFT  = KMOD_LSHIFT;
	constexpr unsigned short KEYMOD_R_SHIFT  = KMOD_RSHIFT;
	constexpr unsigned short KEYMOD_L_CTRL   = KMOD_LCTRL;
	constexpr unsigned short KEYMOD_R_CTRL   = KMOD_RCTRL;
	constexpr unsigned short KEYMOD_L_ALT    = KMOD_LALT;
	constexpr unsigned short KEYMOD_R_ALT    = KMOD_RALT;
	constexpr unsigned short KEYMOD_NUM      = KMOD_NUM;
	constexpr unsigned short KEYMOD_CAPS     = KMOD_CAPS;
	constexpr unsigned short KEYMOD_SHIFT    = KMOD_SHIFT;
	constexpr unsigned short KEYMOD_CTRL     = KMOD_CTRL;
	constexpr unsigned short KEYMOD_ALT      = KMOD_ALT;

	struct mouse_motion_e {
		int x, y, rel_x, rel_y;
	};

	struct mouse_button_e {
		unsigned char button, state, clicks;
		int x, y;
	};

	//Mouse buttons
	constexpr unsigned char MOUSE_LEFT   = SDL_BUTTON_LEFT;
	constexpr unsigned char MOUSE_MIDDLE = SDL_BUTTON_MIDDLE;
	constexpr unsigned char MOUSE_RIGHT  = SDL_BUTTON_RIGHT;

	//States
	constexpr unsigned char MOUSE_DOWN = SDL_PRESSED;
	constexpr unsigned char MOUSE_UP   = SDL_RELEASED;

	struct mouse_wheel_e {
		int x, y;
	};

	class Context {
	private:
		int width, height, blending_mode, aa_mode;
		float rendering_scale;
		bool vsync;
		Colour window_colour;
		std::string window_name;

		std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)> window;
		std::unique_ptr<SDL_Renderer, decltype(&SDL_DestroyRenderer)> renderer;
		SDL_Event event;

		SDL_BlendMode get_blending_mode_internal();
		void print_sdl_error();

	public:
		Context(int window_width, int window_height, const char* window_name_);
		//~Context() noexcept;

		void clear();
		void draw();
		void set_window_colour(Colour w_colour);

		keyboard_e* check_keyboard();
		mouse_motion_e* check_mouse_motion();
		mouse_button_e* check_mouse_button();
		mouse_wheel_e* check_mouse_wheel();
		bool check_exit();
		void clean_events();

		void set_vsync(bool vsync_mode);
		bool get_vsync();

		void  set_scale(float new_scale);
		float get_scale();

		void set_blending_mode(int new_blending_mode);
		int  get_blending_mode();

		void set_aa_mode(int new_aa_mode);
		int  get_aa_mode();

		SDL_Renderer* get_renderer();

		void draw_rect(int x, int y, int w, int h, Colour c, bool filled);
		void draw_line(int x1, int y1, int x2, int y2, Colour c);
		void draw_circle(int x1, int y2, int radius, Colour c);
	};
}