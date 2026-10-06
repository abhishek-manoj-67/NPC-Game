#pragma once

#include <iostream>

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_image/SDL_image.h>

#include "defs.hpp"

#include "Vec2.hpp"
#include "Polygon.hpp"

/*
Establish a strict winding order for polygons:
Counterclockwise
*/
class Entity {
protected:
    Vec2 fNet;

public:
    // constructor
    Entity(const Vec2& p, const Vec2& s, float m = 1);
    
    // methods
    void update(float dt); // update
    void renderHitbox(SDL_Renderer* ren, uint32_t col); // draw

    void resetForce();
    void addForce(const Vec2& force);

    // attrs
    float mass;
    Vec2 pos;
    Vec2 size;

    Vec2 vel;
    Vec2 accel;
};

