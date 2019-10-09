#pragma once

#include <SDL.h>
#include <SDL_ttf.h>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <memory>
#include <algorithm>
#include <unordered_map>

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
		std::unique_ptr<TTF_Font, decltype(&TTF_CloseFont)> font;
		int space_width;
		std::string font_path;


		struct word_identifier {
			std::string word;
			int size;

			bool operator == (const word_identifier& p) const { 
				return word == p.word && size == p.size; 
			} 
		};
		struct cached_word {
			std::string word;
			int width, height, size;
			SDL_Texture* texture;
		};

		struct cached_word_hash {
			size_t operator() (const Text_context::word_identifier& word) const{
				return std::hash<std::string>{}(word.word) ^ (std::hash<int>{}(word.size) << 1);
			}
		};

		std::unordered_map<Text_context::word_identifier, Text_context::cached_word, cached_word_hash> cached_words;

		std::vector<std::string> split(std::string input);

		void print_sdl_error();

	public:
		Text_context(const char* font_path);

		void draw_text(Context* ctx, int x, int y, std::string text, int size);
		void draw_text(Context* ctx, int x, int y, std::string text, int size, font_colour c);
	};
}