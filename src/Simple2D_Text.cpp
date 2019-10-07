#include "Simple2D_Text.h"

namespace Simple2D {
	Text_context::Text_context(const char* font_path_) {
		font_path = font_path_;
		if(TTF_Init() < 0) std::cout << "ERROR: " << TTF_GetError() << '\n';
		font = TTF_OpenFont(font_path_, 24);
		if(font == nullptr) std::cout << "ERROR: " << TTF_GetError() << '\n';
		TTF_GlyphMetrics(font, ' ', nullptr, nullptr, nullptr, nullptr, &space_width);
	}

	Text_context::~Text_context() {
		TTF_CloseFont(font);
		for(auto i : cached_words) {
			SDL_DestroyTexture(i.texture);
		}
	}

	std::vector<std::string> Text_context::split(std::string input) {
		std::string str(input);
		std::string buf;
		std::stringstream ss(str);

		std::vector<std::string> tokens;

		while (ss >> buf)
				tokens.push_back(buf);

		return tokens;
	}

	Text_context::cached_word* Text_context::is_cached(std::string word, int size) {
		for(unsigned int i = 0; i < cached_words.size(); i++) {
			if(word == cached_words[i].word && size == cached_words[i].size) {
				return &cached_words[i];
			}
		}
		return nullptr;
	}

	void Text_context::draw_text(Context* ctx, int x, int y, std::string text, int size) {
		font_colour white = {255, 255, 255, 255};
		draw_text(ctx, x, y, text, size, white);
	}

	void Text_context::draw_text(Context* ctx, int x, int y, std::string text, int size, font_colour c) {
		std::vector<std::string> split_input = split(text);

		if(cached_words.size() > 200)
			cached_words.erase(cached_words.begin());

		for(auto& i : split_input) {
			cached_word* cached = is_cached(i, size);
			if(cached != nullptr) {
				SDL_Rect dest = {x, y, cached->width, cached->height};
				SDL_SetTextureColorMod(cached->texture, c.red, c.green, c.blue);
				SDL_SetTextureAlphaMod(cached->texture, c.alpha);
				SDL_RenderCopy(ctx->get_renderer(), cached->texture, nullptr, &dest);
				x += cached->width;
			}else{
				SDL_Surface* text_surface;
				if(size == 24) {
					text_surface = TTF_RenderUTF8_Blended(font, i.c_str(), {255, 255, 255, 255});
				}else{
					TTF_Font* tmp_font = TTF_OpenFont(font_path.c_str(), size);
					text_surface = TTF_RenderUTF8_Blended(tmp_font, i.c_str(), {255, 255, 255, 255});
					TTF_CloseFont(tmp_font);
				}			
				SDL_Texture* text_texture = SDL_CreateTextureFromSurface(ctx->get_renderer(), text_surface);
				SDL_FreeSurface(text_surface);
				SDL_SetTextureAlphaMod(text_texture, c.alpha);
				SDL_SetTextureColorMod(text_texture, c.red, c.green, c.blue);

				int width, height;
				SDL_QueryTexture(text_texture, nullptr, nullptr, &width, &height);
				SDL_Rect dest = {x, y, width, height};
				SDL_RenderCopy(ctx->get_renderer(), text_texture, nullptr, &dest);
				x += width;
				cached_words.push_back({i, text_texture, width, height, size});
			}
			x += space_width;
		}
	}
}