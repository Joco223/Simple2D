#include "Simple2D_Text.h"

namespace Simple2D {

	void error_ttf_out(const std::string& error) {
		std::cout << "ERROR: " << SDL_GetError() << " - " << error << '\n';
		std::exit(0);
	}

	Text_context::Text_context(const char* font_path_) : font(nullptr, TTF_CloseFont) {
		font_path = font_path_;
		if(TTF_Init() < 0) error_ttf_out("Unable to initialize text library.");
		font.reset(TTF_OpenFont(font_path_, 24));
		if(font == nullptr) error_ttf_out("Unable to load font.");
		TTF_GlyphMetrics(font.get(), ' ', nullptr, nullptr, nullptr, nullptr, &space_width);
	}

	std::vector<std::string> Text_context::split(const std::string& input) {
		std::string str(input);
		std::string buf;
		std::stringstream ss(str);

		std::vector<std::string> tokens;

		while (ss >> buf)
				tokens.push_back(buf);

		return tokens;
	}

	void Text_context::draw_text(const Context* ctx, int x, int y, const std::string& text, int size) {
		font_colour white = {255, 255, 255, 255};
		draw_text(ctx, x, y, text, size, white);
	}

	void Text_context::draw_text(const Context* ctx, int x, int y, const std::string& text, int size, font_colour c) {
		std::vector<std::string> split_input = split(text);

		if(cached_words.size() > 200)
			cached_words.erase(cached_words.begin());

		for(auto& i : split_input) {
			auto cached = cached_words.find({i, size});
			if(cached != cached_words.end()) {
				SDL_Rect dest = {x, y, cached->second.width, cached->second.height};
				if(SDL_SetTextureColorMod(cached->second.texture.get(), c.red, c.green, c.blue) < 0) error_out("Unable to set text colour.");
				if(SDL_SetTextureAlphaMod(cached->second.texture.get(), c.alpha) < 0) error_out("Unable to set text alhpa.");
				if(SDL_RenderCopy(ctx->get_renderer(), cached->second.texture.get(), nullptr, &dest) < 0) error_out("Unable to render text.");
				x += cached->second.width;
			}else{
				SDL_Surface* text_surface;
				if(size == 24) {
					text_surface = TTF_RenderUTF8_Blended(font.get(), i.c_str(), {255, 255, 255, 255});
					if(text_surface == nullptr) error_ttf_out("Unable to render text.");
				}else{
					TTF_Font* tmp_font = TTF_OpenFont(font_path.c_str(), size);
					if(tmp_font == nullptr) error_ttf_out("Unable to open font.");
					text_surface = TTF_RenderUTF8_Blended(tmp_font, i.c_str(), {255, 255, 255, 255});
					if(text_surface == nullptr) error_ttf_out("Unable to render text.");
					TTF_CloseFont(tmp_font);
				}			
				SDL_Texture* text_texture = SDL_CreateTextureFromSurface(ctx->get_renderer(), text_surface);
				if(text_texture == nullptr) error_out("Unbale to create text texture.");
				SDL_FreeSurface(text_surface);
				if(SDL_SetTextureAlphaMod(text_texture, c.alpha) < 0) error_out("Unable to set text colour.");
				if(SDL_SetTextureColorMod(text_texture, c.red, c.green, c.blue) < 0) error_out("Unable to set text alhpa.");

				int width, height;
				if(SDL_QueryTexture(text_texture, nullptr, nullptr, &width, &height) < 0)  error_out("Unable to get word texture info.");
				SDL_Rect dest = {x, y, width, height};
				if(SDL_RenderCopy(ctx->get_renderer(), text_texture, nullptr, &dest) < 0)  error_out("Unable to render text.");
				x += width;
				cached_words.emplace(std::make_pair(word_identifier(i, size), cached_word(i, width, height, size, text_texture)));
			}
			x += space_width;
		}
	}
}