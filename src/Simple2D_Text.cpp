#include "Simple2D_Text.h"

namespace Simple2D {
	Text_context::Text_context(const char* font_path_) : font(nullptr, TTF_CloseFont) {
		font_path = font_path_;
		if(TTF_Init() < 0) std::cout << "ERROR: " << TTF_GetError() << '\n';
		font.reset(TTF_OpenFont(font_path_, 24));
		if(font == nullptr) std::cout << "ERROR: " << TTF_GetError() << '\n';
		TTF_GlyphMetrics(font.get(), ' ', nullptr, nullptr, nullptr, nullptr, &space_width);
	}

	void Text_context::print_sdl_error() {
		std::cout << "ERROR: " << SDL_GetError() << '\n';
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

	void Text_context::draw_text(Context* ctx, int x, int y, std::string text, int size) {
		font_colour white = {255, 255, 255, 255};
		draw_text(ctx, x, y, text, size, white);
	}

	void Text_context::draw_text(Context* ctx, int x, int y, std::string text, int size, font_colour c) {
		std::vector<std::string> split_input = split(text);

		if(cached_words.size() > 200)
			cached_words.erase(cached_words.begin());

		for(auto& i : split_input) {
			if(cached_words.find({i, size}) != cached_words.end()) {
				cached_word cached = cached_words[{i, size}];
				SDL_Rect dest = {x, y, cached.width, cached.height};
				if(SDL_SetTextureColorMod(cached.texture, c.red, c.green, c.blue) < 0) print_sdl_error();
				if(SDL_SetTextureAlphaMod(cached.texture, c.alpha) < 0) print_sdl_error();
				if(SDL_RenderCopy(ctx->get_renderer(), cached.texture, nullptr, &dest) < 0) print_sdl_error();
				x += cached.width;
			}else{
				SDL_Surface* text_surface;
				if(size == 24) {
					text_surface = TTF_RenderUTF8_Blended(font.get(), i.c_str(), {255, 255, 255, 255});
					if(text_surface == nullptr) std::cout << "ERROR: " << TTF_GetError() << '\n';
				}else{
					TTF_Font* tmp_font = TTF_OpenFont(font_path.c_str(), size);
					if(tmp_font == nullptr) std::cout << "ERROR: " << TTF_GetError() << '\n';
					text_surface = TTF_RenderUTF8_Blended(tmp_font, i.c_str(), {255, 255, 255, 255});
					if(text_surface == nullptr) std::cout << "ERROR: " << TTF_GetError() << '\n';
					TTF_CloseFont(tmp_font);
				}			
				SDL_Texture* text_texture = SDL_CreateTextureFromSurface(ctx->get_renderer(), text_surface);
				if(text_texture == nullptr) print_sdl_error();
				SDL_FreeSurface(text_surface);
				if(SDL_SetTextureAlphaMod(text_texture, c.alpha) < 0) print_sdl_error();
				if(SDL_SetTextureColorMod(text_texture, c.red, c.green, c.blue) < 0) print_sdl_error();

				int width, height;
				if(SDL_QueryTexture(text_texture, nullptr, nullptr, &width, &height) < 0)  print_sdl_error();
				SDL_Rect dest = {x, y, width, height};
				if(SDL_RenderCopy(ctx->get_renderer(), text_texture, nullptr, &dest) < 0)  print_sdl_error();
				x += width;
				cached_words[{i, size}] = {i, width, height, size, text_texture};
			}
			x += space_width;
		}
	}
}