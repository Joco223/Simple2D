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
	int new_x = 0;
	int new_y = 0;

	while(!ctx.check_exit()) { //Check if quit button has been pressed

		//Check for keyboard events
		Simple2D::mouse_motion_e mouse_motion_event;
		if(ctx.check_mouse_motion(mouse_motion_event)) {
			new_x = mouse_motion_event.x;
			new_y = mouse_motion_event.y;
		}
		//Clear the screen

		ctx.clear();

		ctx.draw_square_line(50, 50, new_x, new_y, {255, 255, 255, 255});

		test.draw_rotated(&ctx, 200, 200, angle);
		angle += 2;
		angle %= 360;

		//Update the screen
		ctx.draw();
	}

	return 0;
}
