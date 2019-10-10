#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <memory>

#include "Simple2D.h"

namespace Simple2D {

	extern void error_out(const std::string& error);
	
	class Sprite {
	private:
		int width, height, blending_mode;
		std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)> texture;

		SDL_BlendMode get_blending_mode_internal();
	public:
		Sprite(const Context& ctx, const char* texture_file);

		void load_sprite(const Context& ctx,  const char* texture_file);
		int get_sprite_width();
		int get_sprite_height();

		void set_blending_mode(int new_blending_mode);
		int  get_blending_mode();

		void draw(const Context& ctx, int x, int y);
		void draw_rotated(const Context& ctx, int x, int y, double angle);
	};
}