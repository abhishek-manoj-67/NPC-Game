#include <iostream>

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_image/SDL_image.h>

#include "defs.hpp"

#include "Entity.hpp"
#include "Vec2.hpp"
#include "Polygon.hpp"

Entity::Entity(const Vec2& p, const Vec2& s, float m) {

    pos = p;
    size = s;
    mass = m;

}

void Entity::update(float dt) {

    accel = fNet / mass;

    vel += accel * dt;
    pos += vel * dt;

}

void Entity::renderHitbox(SDL_Renderer *ren, uint32_t col) {

    SDL_SetRenderDrawColor(ren, r(col), g(col), b(col), a(col));
    SDL_FRect dest = {pos.x, pos.y, size.x, size.y};
    SDL_RenderFillRect(ren, &dest);

}

void Entity::resetForce() {

    fNet = Vec2(0, 0);

}

void Entity::addForce(const Vec2& force) {
    
    fNet += force;

}
