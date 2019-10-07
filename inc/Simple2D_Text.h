#pragma once

#include <SDL.h>
#include <SDL_ttf.h>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>

#include "Simple2D.h"

struct font_colour {
	unsigned char red;
	unsigned char green;
	unsigned char blue;
	unsigned char alpha;
};

namespace Simple2D {
	class Text_context {
	private:
		TTF_Font* font;
		int space_width;
		std::string font_path;

		struct cached_word {
			std::string word;
			SDL_Texture* texture;
			int width, height, size;
		};

		std::vector<Text_context::cached_word> cached_words;

		std::vector<std::string> split(std::string input);
		Text_context::cached_word* is_cached(std::string word, int size);

		void print_sdl_error();

	public:
		Text_context(const char* font_path);
		~Text_context();

		void draw_text(Context* ctx, int x, int y, std::string text, int size);
		void draw_text(Context* ctx, int x, int y, std::string text, int size, font_colour c);
	};
}