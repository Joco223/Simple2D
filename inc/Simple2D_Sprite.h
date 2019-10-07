#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <iostream>

#include "Simple2D.h"

namespace Simple2D {
	class Sprite {
	private:
		int width, height, blending_mode;
		SDL_Texture* texture;

		SDL_BlendMode get_blending_mode_internal();
		void print_sdl_error();
	public:
		Sprite(Context* ctx, const char* texture_file);
		~Sprite();

		void load_sprite(Context* ctx,  const char* texture_file);
		int get_sprite_width();
		int get_sprite_height();

		void set_blending_mode(int new_blending_mode);
		int  get_blending_mode();

		void draw(Context* ctx, int x, int y);
		void draw_rotated(Context* ctx, int x, int y, double angle);
	};
}