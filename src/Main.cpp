#define SDL_MAIN_HANDLED

#include "Simple2D.h"
#include "Simple2D_Sprite.h"
#include "Simple2D_Text.h"

#include <iostream>
#include <string>

constexpr int width                      = 640;
constexpr int height                     = 480;
constexpr Simple2D::Colour window_colour = {50, 50, 50, 255};
constexpr float scale                    = 1;

int main() {

	Simple2D::Context ctx(width, height, "Simple2D test");           //Simple2D Context object, arguments, window width and height, window title
	ctx.set_window_colour(window_colour);                            //Set window background colour to a dark gray one
	ctx.set_vsync(true);                                             //Turn on VSync (Off by default)
	ctx.set_scale(scale);                                            //Set rendering scale to 2x (1 by default)
	ctx.set_blending_mode(Simple2D::S2D_BLENDING_ALPHA);             //Set blending mode to alpha mode (Off by default)
	ctx.set_aa_mode(Simple2D::S2D_AA_NEAREST);                       //Set AA mode to nearest (Nearest by default)

	Simple2D::Sprite test(&ctx, "cobblestone.png");

	Simple2D::Text_context text_ctx("arial.ttf");

	int angle = 0;
	unsigned char alpha = 1;

	std::string printable = "";

	while(!ctx.check_exit()) { //Check if quit button has been pressed

		//Check for keyboard events
		Simple2D::keyboard_e* keyboard_event = ctx.check_keyboard();
		if(keyboard_event != nullptr)
			if(keyboard_event->state == Simple2D::KEY_DOWN)
				printable.push_back(keyboard_event->character);

		//Clear the screen

		ctx.clear();

		text_ctx.draw_text(&ctx, 50, 50, printable, 30, {50, alpha, 123, alpha});
		alpha += 4;
		alpha %= 255;

		test.draw_rotated(&ctx, 200, 200, angle);
		angle += 2;
		angle %= 360;

		//Update the screen
		ctx.draw();
	}

	return 0;
}
