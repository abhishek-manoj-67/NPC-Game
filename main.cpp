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
	Vec2 acceleration(0, 0.98);
	Vec2 velocity(0, 0);
	Vec2 direction(0, 0);

	// create the player itself
	float sX = 90, sY = 0;
	// Polygon player({
	// 	Vec2(1.2361f * apothem, 0.0f),
	// 	Vec2(0.3820f * apothem, -1.1756f * apothem),
	// 	Vec2(-1.0000f * apothem, -0.7265f * apothem),
	// 	Vec2(-1.0000f * apothem, 0.7265f * apothem),
	// 	Vec2(0.3820f * apothem, 1.1756f * apothem)
	// });

	uint32_t color = 0x189bccff;

	Polygon player({
		Vec2(0.0f, 0.0f),
		Vec2(10.0f, 0.0f),
		Vec2(10.0f, 20.0f),
		Vec2(0.0f, 20.0f),
	});

	float speed = 3.0f;
	float jumpHeight = 7.5f;

	for (Vec2& v : player.vertices) {
		v.x += sX;
		v.y += sY;
	}

	vector<Polygon> obstacles;
	// obstacles.push_back(Polygon({Vec2(500, 500), Vec2(600, 400), Vec2(600, 100), Vec2(400, 100), Vec2(400, 400)}));
	// obstacles.push_back(Polygon({Vec2(0, 300), Vec2(100, 200), Vec2(0, 100)}));
	// obstacles.push_back(Polygon({Vec2(80, 400), Vec2(420, 450), Vec2(490, 280)}));
	// obstacles.push_back(Polygon({Vec2(0, 600), Vec2(50, 600), Vec2(0, 400)}));
	// obstacles.push_back(Polygon({Vec2(0, 602), Vec2(300, 602), Vec2(300, 600), Vec2(0, 600)}));
	// obstacles.push_back(Polygon({Vec2(300, 600), Vec2(400, 600), Vec2(400, 500)}));

	obstacles.push_back(Polygon({Vec2(0, 400), 
								 Vec2(100, 400),
								 Vec2(0, 390)}));
	obstacles.push_back(Polygon({Vec2(0, 400), 
								 Vec2(75, 400),
								 Vec2(0, 370)}));
	obstacles.push_back(Polygon({Vec2(0, 400), 
								 Vec2(50, 400),
								 Vec2(0, 330)}));

	obstacles.push_back(Polygon({Vec2(300, 400), 
								 Vec2(400, 400),
								 Vec2(400, 390)}));

	obstacles.push_back(Polygon({Vec2(0, 450), 
								 Vec2(400, 450),
								 Vec2(400, 400),
								 Vec2(0, 400)}));

	obstacles.push_back(Polygon({Vec2(150, 325), 
								 Vec2(300, 325),
								 Vec2(250, 370),
								 Vec2(100, 370)}));
	
	obstacles.push_back(Polygon({Vec2(400, 390), 
								 Vec2(620, 330),
								 Vec2(800, 330),
								 Vec2(800, 450),
								 Vec2(400, 450)}));

	// time based mechanics


    // game loop
    bool done = false;
	Vec2 mouse(0, 0);
	while (!done) {
		// get time at start of frame
		// auto endTime = std::chrono::high_resolution_clock::now();
		// event poll
		SDL_PollEvent(&e);
		SDL_GetMouseState(&mouse.x, &mouse.y);
		switch(e.type) {
			case SDL_EVENT_QUIT:
				done = true;
				break;

			case SDL_EVENT_MOUSE_BUTTON_DOWN:
				switch (e.button.button) {
					case SDL_BUTTON_LEFT:
						std::cout << mouse << std::endl;
						acceleration.y *= -1;

						break;
				}
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
		player.moveBy(velocity);
		// player.moveTo(mouse);
		// player.rotateBy(PI * -0.0125);

		velocity.y += acceleration.y;

		
		direction.x = keys.d - keys.a;
		// direction.y = keys.s - keys.w;
		velocity.x = direction.x * speed;
		
		// uodate direction vector
		for (const Polygon& obstacle : obstacles) {
			Vec2 mtv(0, 0);
			if (player.collidePolygon(obstacle, &mtv)) {
				// set velocity to zero to not have latency
				Vec2 wallNorm = mtv.normalize();
				float dot = wallNorm.dot(velocity);
				player.moveBy(mtv);
				
				velocity -= wallNorm * dot;
				// rotate to the normal of the thingy
				// player pointing angle:
				// Vec2 pointing(std::cos(player.angle), std::sin(player.angle));
				// angle between is |A||B|cos(c) = a1b1 + a2b2
				// but |A| and |B| are one
				// cos(c) = a1b1 + a2b2
				// float dotAngle = pointing.dot(wallNorm);
				// float angle = std::acos(dotAngle);
				// player.rotateTo(angle);

				// get the tangent of the wall normal
				Vec2 wallTan = wallNorm.perpendicular();
				if (velocity.dot(wallTan) < 0) {wallTan *= -1;}

				player.rotateTo(std::atan2(wallTan.y, wallTan.x));
				
				
				// cout << wallNorm << endl;

				if (keys.w && wallNorm.dot(Vec2(0, -acceleration.y)) > 0) {
					velocity.y = sign(acceleration.y) * -jumpHeight;
				}
				
				// velocity.y = 0;
			}
		}

		
		// this aint no game nephew this the streets
		
		// set bg color
		SDL_SetRenderDrawColor(ren, 0x00, 0x00, 0x00, 0xff);
		SDL_RenderClear(ren);

		// draw stuff here
		float i = 0;
		for (const Polygon& obstacle : obstacles) {
			uint32_t r = floor((i + 1) / obstacles.size() * 255);
			// cout << r << endl;

			renderPolygon(ren, obstacle, (r << 24) | 0x000000ff);
			i++;
		}
		renderPolygon(ren, player, color);
		
		// update screen
		SDL_RenderPresent(ren);
		SDL_Delay(16);

		// debug
		// cout << player.angle << endl;
	}

	// close window & renderer
	SDL_DestroyWindow(win);
	SDL_DestroyRenderer(ren);

	// quit
	SDL_Quit();

	return 0;
}
