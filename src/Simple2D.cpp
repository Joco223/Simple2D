#include "Simple2D.h"

namespace Simple2D {
	Context::Context(int window_width, int window_height, const char* window_name_) 
	: width(window_width), 
	  height(window_height),
		blending_mode(0),
		aa_mode(0),
		rendering_scale(1),
		vsync(false),	
		window_colour({0, 0, 0, 255}),
		window_name(window_name_),
		window(nullptr, SDL_DestroyWindow),
		renderer(nullptr, SDL_DestroyRenderer) {

		window.reset(SDL_CreateWindow(window_name_, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, window_width, window_height, SDL_WINDOW_SHOWN));
		if(window.get() == nullptr) print_sdl_error();

		renderer.reset(SDL_CreateRenderer(window.get(), -1, SDL_RENDERER_ACCELERATED));
		if(renderer.get() == nullptr) print_sdl_error();

		if(SDL_RenderSetScale(renderer.get(), rendering_scale, rendering_scale) < 0) print_sdl_error();

		if(SDL_SetRenderDrawBlendMode(renderer.get(), SDL_BLENDMODE_NONE) < 0) print_sdl_error();

		if(SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, std::to_string(aa_mode).c_str()) < 0) print_sdl_error();
	}

	void Context::print_sdl_error() {
		std::cout << "ERROR: " << SDL_GetError() << '\n';
	}

	SDL_BlendMode Context::get_blending_mode_internal() {
		switch(blending_mode) {
			case 0: return SDL_BLENDMODE_NONE;
			case 1: return SDL_BLENDMODE_BLEND;
			case 2: return SDL_BLENDMODE_ADD;
			case 3: return SDL_BLENDMODE_MOD;
			default: std::cout << "ERROR: Invalid window blending mode " << blending_mode << '\n'; return SDL_BLENDMODE_NONE;
		}
	}

	void Context::clear() {
		if(SDL_RenderClear(renderer.get()) < 0) print_sdl_error();
	}

	void Context::draw() {
		if(SDL_SetRenderDrawColor(renderer.get(), window_colour.red, window_colour.green, window_colour.blue, window_colour.alpha) < 0) print_sdl_error();
		SDL_RenderPresent(renderer.get());
	}

	keyboard_e* Context::check_keyboard() {
		SDL_PumpEvents();
		keyboard_e* new_event = nullptr;
		if(SDL_PeepEvents(&event, 1, SDL_PEEKEVENT, SDL_KEYDOWN, SDL_KEYUP) > 0) {
			new_event = new keyboard_e;
			new_event->state = event.key.state; 
			new_event->key_code = event.key.keysym.sym; 
			new_event->mod = event.key.keysym.mod;
			new_event->character = (unsigned char)event.key.keysym.sym;
			SDL_PeepEvents(&event, 1, SDL_GETEVENT, SDL_KEYDOWN, SDL_KEYUP);		
		}
		return new_event;
	}

	mouse_motion_e* Context::check_mouse_motion() {
		SDL_PumpEvents();
		mouse_motion_e* new_event = nullptr;
		if(SDL_PeepEvents(&event, 1, SDL_PEEKEVENT, SDL_MOUSEMOTION, SDL_MOUSEMOTION) > 0) {
			new_event = new mouse_motion_e;
			new_event->x = event.motion.x; 
			new_event->y = event.motion.y; 
			new_event->rel_x = event.motion.xrel;
			new_event->rel_y = event.motion.yrel;
			SDL_PeepEvents(&event, 1, SDL_GETEVENT, SDL_MOUSEMOTION, SDL_MOUSEMOTION);		
		}
		return new_event;
	}

	mouse_button_e* Context::check_mouse_button() {
		SDL_PumpEvents();
		mouse_button_e* new_event = nullptr;
		if(SDL_PeepEvents(&event, 1, SDL_PEEKEVENT, SDL_MOUSEBUTTONDOWN, SDL_MOUSEBUTTONUP) > 0) {
			new_event = new mouse_button_e;
			new_event->x = event.button.x; 
			new_event->y = event.button.y; 
			new_event->state = event.button.state;
			new_event->clicks = event.button.clicks;
			new_event->button = event.button.button;
			SDL_PeepEvents(&event, 1, SDL_GETEVENT, SDL_MOUSEBUTTONDOWN, SDL_MOUSEBUTTONUP);		
		}
		return new_event;
	}

	mouse_wheel_e* Context::check_mouse_wheel() {
		SDL_PumpEvents();
		mouse_wheel_e* new_event = nullptr;
		if(SDL_PeepEvents(&event, 1, SDL_PEEKEVENT, SDL_MOUSEWHEEL, SDL_MOUSEWHEEL) > 0) {
			new_event = new mouse_wheel_e;
			new_event->x = event.wheel.x; 
			new_event->y = event.wheel.y; 
			SDL_PeepEvents(&event, 1, SDL_GETEVENT, SDL_MOUSEWHEEL, SDL_MOUSEWHEEL);		
		}
		return new_event;
	}
	
	bool Context::check_exit() {
		SDL_PumpEvents();
		if(SDL_PeepEvents(&event, 1, SDL_PEEKEVENT, SDL_QUIT, SDL_QUIT) > 0) {
			SDL_PeepEvents(&event, 1, SDL_GETEVENT, SDL_QUIT, SDL_QUIT);
			return true;
		}
		return false;
	}

	void Context::clean_events() {
		while(SDL_PeepEvents(&event, 1, SDL_PEEKEVENT, SDL_KEYDOWN, SDL_KEYUP) == 0 &&
		      SDL_PeepEvents(&event, 1, SDL_PEEKEVENT, SDL_MOUSEMOTION, SDL_MOUSEMOTION) == 0 &&
					SDL_PeepEvents(&event, 1, SDL_PEEKEVENT, SDL_MOUSEBUTTONDOWN, SDL_MOUSEBUTTONUP) == 0 &&
					SDL_PeepEvents(&event, 1, SDL_PEEKEVENT, SDL_MOUSEWHEEL, SDL_MOUSEWHEEL) == 0 &&
					SDL_PeepEvents(&event, 1, SDL_PEEKEVENT, SDL_QUIT, SDL_QUIT) == 0) {
						SDL_PollEvent(&event);
					}
	}

	void Context::set_window_colour(Colour new_window_colour) {
		window_colour = new_window_colour;
	}

	void Context::set_vsync(bool vsync_mode) {
		renderer.reset();
		if(vsync_mode) {
			renderer.reset(SDL_CreateRenderer(window.get(), -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC));
		}else{
			renderer.reset(SDL_CreateRenderer(window.get(), -1, SDL_RENDERER_ACCELERATED));
		}
		if(renderer.get() == nullptr) print_sdl_error();
		vsync = vsync_mode;
	}

	bool Context::get_vsync() {
		return vsync;
	}

	void Context::set_scale(float new_scale) {
		rendering_scale = new_scale;
		if(SDL_RenderSetScale(renderer.get(), rendering_scale, rendering_scale) < 0) print_sdl_error();
		if(SDL_SetRenderDrawBlendMode(renderer.get(), get_blending_mode_internal()) < 0) print_sdl_error();
	}

	float Context::get_scale() {
		return rendering_scale;
	}

	void Context::set_blending_mode(int new_blending_mode) {
		if(new_blending_mode != 0 && new_blending_mode != 1 && new_blending_mode != 2 && new_blending_mode != 3) std::cout << "ERROR: Ivalid blending mode " << new_blending_mode << '\n';
		blending_mode = new_blending_mode;
		if(SDL_SetRenderDrawBlendMode(renderer.get(), get_blending_mode_internal()) < 0) print_sdl_error();
	}

	int Context::get_blending_mode() {
		return blending_mode;
	}

	void Context::set_aa_mode(int new_aa_mode) {
		if(new_aa_mode != 0 && new_aa_mode != 1 && new_aa_mode != 2) std::cout << "ERROR: Invalid AA mode " << new_aa_mode << '\n';
		aa_mode = new_aa_mode;
		if(SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, std::to_string(aa_mode).c_str()) < 0) print_sdl_error();
	}

	int Context::get_aa_mode() {
		return aa_mode;
	}

	SDL_Renderer* Context::get_renderer() {
		return renderer.get();
	}

	void Context::draw_rect(int x, int y, int w, int h, Colour c, bool filled) {
		if(SDL_SetRenderDrawColor(renderer.get(), c.red, c.green, c.blue, c.alpha) < 0) print_sdl_error();
		SDL_Rect rect = {x, y, w, h};
		if(filled) {
			if(SDL_RenderFillRect(renderer.get(), &rect) < 0) print_sdl_error();
		}else{
			if(SDL_RenderDrawRect(renderer.get(), &rect) < 0) print_sdl_error();
		}
	}

	void Context::draw_line(int x1, int y1, int x2, int y2, Colour c) {
		if(SDL_SetRenderDrawColor(renderer.get(), c.red, c.green, c.blue, c.alpha) < 0) print_sdl_error();

		bool steep = (fabs(y2 - y1) > fabs(x2 - x1));
		if(steep) {
			std::swap(x1, y1);
			std::swap(x2, y2);
		}

		if(x1 > x2) {
			std::swap(x1, x2);
			std::swap(y1, y2);
		}

		float dx = x2 - x1;
		float dy = fabs(y2 - y1);

		float error = dx / 2.0f;
		int ystep = (y1 < y2) ? 1 : -1;
		int y = (int)y1;

		const int maxX = (int)x2;

		for(int x = (int)x1; x < maxX; x++) {
			if(steep) {
				if(SDL_RenderDrawPoint(renderer.get(), x, y) < 0) print_sdl_error();
			}else{
				if(SDL_RenderDrawPoint(renderer.get(), x, y) < 0) print_sdl_error();
			}

			error -= dy;
			if(error < 0) {
				y += ystep;
				error += dx;
			}
		}
	}

	void Context::draw_circle(int x1, int y1, int radius, Colour c) {
		if(SDL_SetRenderDrawColor(renderer.get(), c.red, c.green, c.blue, c.alpha) < 0) print_sdl_error();

		int x = radius-1;
		int y = 0;
		int dx = 1;
		int dy = 1;
		int err = dx - (radius << 1);

		while (x >= y) {
			if(SDL_RenderDrawPoint(renderer.get(), x1 + x, y1 + y) < 0) print_sdl_error();
			if(SDL_RenderDrawPoint(renderer.get(), x1 + y, y1 + x) < 0) print_sdl_error();
			if(SDL_RenderDrawPoint(renderer.get(), x1 - y, y1 + x) < 0) print_sdl_error();
			if(SDL_RenderDrawPoint(renderer.get(), x1 - x, y1 + y) < 0) print_sdl_error();
			if(SDL_RenderDrawPoint(renderer.get(), x1 - x, y1 - y) < 0) print_sdl_error();
			if(SDL_RenderDrawPoint(renderer.get(), x1 - y, y1 - x) < 0) print_sdl_error();
			if(SDL_RenderDrawPoint(renderer.get(), x1 + y, y1 - x) < 0) print_sdl_error();
			if(SDL_RenderDrawPoint(renderer.get(), x1 + x, y1 - y) < 0) print_sdl_error();

			if(err <= 0) {
				y++;
				err += dy;
				dy += 2;
			}
			
			if(err > 0) {
				x--;
				dx += 2;
				err += dx - (radius << 1);
			}
		}
	}
}