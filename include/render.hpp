#include <iostream>
#include <vector>
#include <string>

#include "SDL3/SDL.h"
#include <SDL3/SDL_main.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_mixer/SDL_mixer.h>

#include "defs.hpp"
#include "Vec2.hpp"
#include "Polygon.hpp"
#include "Entity.hpp"

void renderPolygon(SDL_Renderer* ren, const Polygon& poly, uint32_t color) {

	if (poly.vertices.size() < 3) { return; }

	int verts = poly.vertices.size();

    std::vector<SDL_Vertex> vertices(verts);
	for (int i = 0; i < verts; i++) {

		vertices[i].position.x = poly.vertices[i].x;
		vertices[i].position.y = poly.vertices[i].y;
		vertices[i].color.r = r(color) / 255.0f; vertices[i].color.g = g(color) / 255.0f;
		vertices[i].color.b = b(color) / 255.0f; vertices[i].color.a = a(color) / 255.0f;
	}

	int inds = (verts - 2) * 3;
	std::vector<int> indices(inds);

	int cidx = 0;
	for (int i = 1; i < verts - 1; i++) {
		indices[cidx] = 0;
		indices[cidx + 1] = i;
		indices[cidx + 2] = i + 1;
		cidx += 3;
	}

	SDL_RenderGeometry(ren, nullptr, vertices.data(), verts, indices.data(), inds);
}
