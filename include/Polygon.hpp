#pragma once

#include <iostream>
#include <vector>

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_image/SDL_image.h>

#include "defs.hpp"

#include "Vec2.hpp"

class Polygon {
public:
    // constructor
    Polygon(std::initializer_list<Vec2> vs);

    // methods
    bool collidePolygon(const Polygon& other, Vec2* mtvOut = nullptr) const;
    Vec2 centroid() const;

    void moveTo(const Vec2& dest);
    void moveBy(const Vec2& dv);

    // attrs
    std::vector<Vec2> vertices;
    float angle;
};
