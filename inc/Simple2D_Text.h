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

	extern void error_out(const std::string& error);
	void error_ttf_out(const std::string& error);

	class Text_context {
	private:
		std::unique_ptr<TTF_Font, decltype(&TTF_CloseFont)> font;
		int space_width;
		std::string font_path;

		struct word_identifier {
			std::string word;
			int size;

			word_identifier(std::string word_, int size_)
			:
			word(word_),
			size(size_) {}

			bool operator == (const word_identifier& p) const { 
				return word == p.word && size == p.size; 
			} 
		};
		struct cached_word {
			std::string word;
			int width, height, size;
			std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)> texture;

			cached_word(std::string word_, int width_, int height_, int size_, SDL_Texture* texture_)
			:
			word(word_),
			width(width_),
			height(height_),
			size(size_),
			texture(nullptr, SDL_DestroyTexture) {
				texture.reset(texture_);
			}
		};

		struct cached_word_hash {
			size_t operator() (const Text_context::word_identifier& word) const{
				return std::hash<std::string>{}(word.word) ^ (std::hash<int>{}(word.size) << 1);
			}
		};

		std::unordered_map<Text_context::word_identifier, Text_context::cached_word, cached_word_hash> cached_words;

		std::vector<std::string> split(const std::string& input);

	public:
		Text_context(const char* font_path);

		void draw_text(const Context& ctx, int x, int y, const std::string& text, int size);
		void draw_text(const Context& ctx, int x, int y, const std::string& text, int size, font_colour c);
	};
}