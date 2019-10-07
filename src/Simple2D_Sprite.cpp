#include "Simple2D_Sprite.h"

namespace Simple2D {
	Sprite::Sprite(Context* ctx, const char* texture_file) {
		texture = IMG_LoadTexture(ctx->get_renderer(), texture_file);
		if(texture == nullptr) print_sdl_error();
		if(SDL_QueryTexture(texture, nullptr, nullptr, &width, &height) < 0) print_sdl_error();
		if(SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_NONE) < 0) print_sdl_error();
	}

	Sprite::~Sprite() {
		SDL_DestroyTexture(texture);
	}

	void Sprite::print_sdl_error() {
		std::cout << "ERROR: " << SDL_GetError() << '\n';
	}

	SDL_BlendMode Sprite::get_blending_mode_internal() {
		switch(blending_mode) {
			case 0: return SDL_BLENDMODE_NONE;
			case 1: return SDL_BLENDMODE_BLEND;
			case 2: return SDL_BLENDMODE_ADD;
			case 3: return SDL_BLENDMODE_MOD;
			default: std::cout << "ERROR: Invalid window blending mode " << blending_mode << '\n'; return SDL_BLENDMODE_NONE;
		}
	}

	void Sprite::load_sprite(Context* ctx, const char* texture_file) {
		SDL_DestroyTexture(texture);
		texture = IMG_LoadTexture(ctx->get_renderer(), texture_file);
		if(texture == nullptr) print_sdl_error();
		if(SDL_QueryTexture(texture, nullptr, nullptr, &width, &height) < 0) print_sdl_error();
		if(SDL_SetTextureBlendMode(texture, get_blending_mode_internal()) < 0) print_sdl_error();
	}

	int Sprite::get_sprite_height() {
		return height;
	}

	int Sprite::get_sprite_width() {
		return width;
	}

	void Sprite::set_blending_mode(int new_blending_mode) {
		if(new_blending_mode != 0 && new_blending_mode != 1 && new_blending_mode != 2 && new_blending_mode != 3) std::cout << "ERROR: Ivalid blending mode " << new_blending_mode << '\n';
		blending_mode = new_blending_mode;
		if(SDL_SetTextureBlendMode(texture, get_blending_mode_internal()) < 0) print_sdl_error();
	}

	int Sprite::get_blending_mode() {
		return blending_mode;
	}

	void Sprite::draw(Context* ctx, int x, int y) {
		SDL_Rect destination;
		destination.x = x;
		destination.y = y;
		destination.w = width;
		destination.h = height;
		if(SDL_RenderCopy(ctx->get_renderer(), texture, nullptr, &destination) < 0) print_sdl_error();
	}

	void Sprite::draw_rotated(Context* ctx, int x, int y, double angle) {
		SDL_Rect destination;
		destination.x = x;
		destination.y = y;
		destination.w = width;
		destination.h = height;
		SDL_Point rotation_center;
		rotation_center.x = width/2;
		rotation_center.y = height/2;
		if(SDL_RenderCopyEx(ctx->get_renderer(), texture, nullptr, &destination, angle, &rotation_center, SDL_FLIP_NONE) < 0) print_sdl_error();
	}
}