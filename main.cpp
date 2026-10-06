#include <iostream>
#include <vector>
#include <string>

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_mixer/SDL_mixer.h>

#include "include/defs.hpp"
#include "include/Vec2.hpp"
#include "include/Polygon.hpp"
#include "include/Entity.hpp"

#include "include/render.hpp"

using std::cout;
using std::vector;
using std::endl;
using std::string;

int main(int argc, char* argv[]) {
	// declare objects
	SDL_Window* win;
	SDL_Renderer* ren;
	SDL_Event e;

	// instantiate window and renderer
	if (!SDL_CreateWindowAndRenderer("impostor simulator", WINWIDTH, WINHEIGHT, SDL_WINDOW_RESIZABLE, &win, &ren)) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create window and renderer: %s", SDL_GetError());
        return 3;
    }

	// setup game

	Polygon polyA({Vec2(0, 100), Vec2(100, 0), Vec2(0, 0)});
	uint32_t normalCol = 0x0000ffff;
	uint32_t overlapCol = 0xff0000ff;
	uint32_t color = normalCol;

	Polygon polyB({Vec2(300, 300), Vec2(400, 300), Vec2(400, 200), Vec2(300, 75)});
	uint32_t bColor = 0xffffffff;

	Vec2 dv(1, 0);
	dv *= 1;

    // game loop
    bool done = false;
	Vec2 mouse(0, 0);
	while (!done) {
		// event poll
		SDL_PollEvent(&e);
		SDL_GetMouseState(&mouse.x, &mouse.y);
		switch(e.type) {
		case SDL_EVENT_QUIT:
			done = true;
			break;
		}

		// update
		polyA.moveBy(dv);
		Vec2 collisionResolution(0, 0);

		// collision
		if (polyA.collidePolygon(polyB, &collisionResolution)) {
			color = overlapCol;
			polyA.moveBy(collisionResolution);
		} else {
			color = normalCol;
		}
		
		// set bg color
		SDL_SetRenderDrawColor(ren, 0x00, 0x00, 0x00, 0xff);
		SDL_RenderClear(ren);

		// draw stuff here
		renderPolygon(ren, polyB, bColor);
		renderPolygon(ren, polyA, color);
		
		// update screen
		SDL_RenderPresent(ren);
		SDL_Delay(16);
	}

	// close window & renderer
	SDL_DestroyWindow(win);
	SDL_DestroyRenderer(ren);

	// quit
	SDL_Quit();

	return 0;
}
