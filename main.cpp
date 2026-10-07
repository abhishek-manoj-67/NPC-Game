#include <iostream>
#include <vector>
#include <string>
#include <cmath>

// time
#include <chrono>

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
	// create keys
	struct {bool w = 0, a = 0, s = 0, d = 0; } keys;
	
	// player attributes such as direction, magnitude of velocity
	Vec2 direction(0, 0);
	float speed = 5.0f;

	// create the player itself
	float apothem = 50;
	Polygon player({
		Vec2(1.2361f * apothem, 0.0f),
		Vec2(0.3820f * apothem, -1.1756f * apothem),
		Vec2(-1.0000f * apothem, -0.7265f * apothem),
		Vec2(-1.0000f * apothem, 0.7265f * apothem),
		Vec2(0.3820f * apothem, 1.1756f * apothem)
	});

	Polygon obstacle({Vec2(500, 500), Vec2(600, 400), Vec2(600, 100), Vec2(400, 100), Vec2(400, 400)});
	Vec2 mtv(0, 0);

	// angle bnetween
	float angleBetween = 0.0f;

	// time based mechanics
	auto startTime = std::chrono::high_resolution_clock::now();

    // game loop
    bool done = false;
	Vec2 mouse(0, 0);
	while (!done) {
		// get time at start of frame
		auto endTime = std::chrono::high_resolution_clock::now();
		// event poll
		SDL_PollEvent(&e);
		SDL_GetMouseState(&mouse.x, &mouse.y);
		switch(e.type) {
			case SDL_EVENT_QUIT:
				done = true;
				break;

			case SDL_EVENT_KEY_DOWN:
				switch (e.key.key) {
					case SDLK_W:
						keys.w = 1;
					break;

					case SDLK_A:
						keys.a = 1;
					break;
					
					case SDLK_S:
						keys.s = 1;
					break;
					
					case SDLK_D:
						keys.d = 1;
					break;
				}
				break;

			case SDL_EVENT_KEY_UP:
				switch (e.key.key) {
					case SDLK_W:
						keys.w = 0;
					break;

					case SDLK_A:
						keys.a = 0;
					break;
					
					case SDLK_S:
						keys.s = 0;
					break;
					
					case SDLK_D:
						keys.d = 0;
					break;
				}
				break;
		}

		// get player center
		Vec2 centroid = player.centroid();

		// get the angle
		angleBetween = std::atan2((mouse.y - centroid.y), (mouse.x - centroid.x));

		// uodate direction vector
		direction = Vec2(keys.d - keys.a, keys.s - keys.w).normalize();

		// update player position and angle
		// obstacle.moveBy(Vec2(-(10 * std::sin(duration.count() / 300)), 10 * std::cos(duration.count() / 200))); // use cosine of time
		player.moveBy(direction * speed);
		player.rotateBy(PI * 0.01);

		if (player.collidePolygon(obstacle, &mtv)) {
			player.moveBy(mtv);
		}
		
		// set bg color
		SDL_SetRenderDrawColor(ren, 0x00, 0x00, 0x00, 0xff);
		SDL_RenderClear(ren);

		// draw stuff here
		renderPolygon(ren, player, 0x189bccff);
		renderPolygon(ren, obstacle, 0xffffffff);

		SDL_SetRenderDrawColor(ren, 0xff, 0xff, 0xff, 0xff);
		// SDL_RenderLine(ren, centroid.x, centroid.y, centroid.x + 50 * std::cos(angleBetween), centroid.y + 50 * std::sin(angleBetween));
		
		// update screen
		SDL_RenderPresent(ren);
		SDL_Delay(16);

		// debug
	}

	// close window & renderer
	SDL_DestroyWindow(win);
	SDL_DestroyRenderer(ren);

	// quit
	SDL_Quit();

	return 0;
}
