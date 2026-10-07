#include <iostream>
#include <cassert>
#include <algorithm>
#include <cmath>

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_image/SDL_image.h>

#include "defs.hpp"

#include "Vec2.hpp"
#include "Polygon.hpp"

// funny thing this took me 152 lines of code to write.
// firstly the collision detection took me from 11 pm to 1 am
// the collision resolution took me 10 am to 11:30 am
// across the span of two days.

// In two days i accomplished what i gave up 5 months ago, which i couldnt get for over a week
// i didnt use ai at all either this time, and i got it

// constructor
Polygon::Polygon(std::initializer_list<Vec2> vs) {

    // make sure that its actually a polygon and not a line segment or point
    assert(vs.size() >= 3);

    for (const Vec2& vert : vs) {
        vertices.push_back(vert);
    }

    angle = 0;

}

// collision detection algorithm
bool Polygon::collidePolygon(const Polygon &other, Vec2* mtvOut) const {   
    // get the edge vectors
    std::vector<Vec2> axes;
    std::vector<float> overlaps;
    axes.resize(vertices.size() + other.vertices.size());
    overlaps.resize(vertices.size() + other.vertices.size());

    // loop through each vertices list, subtract the next from current to get the edge vectors
    int c = 0;
    for (int i = 0; i < vertices.size(); i++) {
        if (i == vertices.size() - 1) {
            axes[c] = (vertices[0] - vertices[i]).perpendicular().normalize();
        } else {
            axes[c] = (vertices[i + 1] - vertices[i]).perpendicular().normalize();
        }
        c++;
    }
    // continue through other's vertex list
    for (int i = 0; i < other.vertices.size(); i++) {
        if (i == other.vertices.size() - 1) {
            axes[c] = (other.vertices[0] - other.vertices[i]).perpendicular().normalize();
        } else {
            axes[c] = (other.vertices[i + 1] - other.vertices[i]).perpendicular().normalize();
        }
        c++;
    }

    // iterate through each axis
    c = 0;
    for (const Vec2& axis : axes) {
        // project shape A (self) onto the axis to get its interval
        float minS = axis.dot(vertices[0]), maxS = axis.dot(vertices[0]);
        for (const Vec2& vertex : vertices) {
            
            float d = axis.dot(vertex);
            if (d < minS) {minS = d;}
            else if (d > maxS) {maxS = d;}

        }
        // project shape B (other) onto the axis to get its interval
        float minO = axis.dot(other.vertices[0]), maxO = axis.dot(other.vertices[0]);
        for (const Vec2& vertex : other.vertices) {
            
            float d = axis.dot(vertex);
            if (d < minO) {minO = d;}
            else if (d > maxO) {maxO = d;}

        }
        // if the interval does NOT have an overlap, we have found an axis that separatrs them, so return false
        /*
        [0 1 2 3 4]
                  [5 6 7 8 9]
        
        [0 1 2 3 4]
            [2 3 4 5 6]
        */

        // this is when there IS no overlap. i avoid having the debug stuff like negative overlap or whatever, because this will
        // just exit once it doesnt find an overlap
        if (maxS <= minO || maxO <= minS) {
            return false;
        }

        // find the overlap, because we have determined that the shapes ARE overlapping
        // the overlap will be a positive scalar magnitude, and since the lists are related, the index at which this overlap exists,
        // the corresponding vector int he axes list will be the vector portion.

        // one case is that interval A starts before interval B
        // in such case, magnitude of overlap is (maxA - minB)
        // the other case is that interval B starts before interval A
        // in such case, magnitude of overlap is (maxB - minA)

        if (minS < minO) { // case where A starts before B
            overlaps[c] = maxS - minO;
        } else { // the only other case where B starts before A
            overlaps[c] = maxO - minS;
        }
        c++; // increment c! don't forget
    }


    // we can only have reached here once we have NOT found a separating axis, so they MUST be overlapping
    // let us find the minimum translation vector and return that to them first
    auto minIterator = std::min_element(overlaps.begin(), overlaps.end());
    float minOverlap = *minIterator; // get the magnitude portion
    int minIndex = std::distance(overlaps.begin(), minIterator); // get the index of where it exists
    // using the index, we KNOW that at index minIndex, it should ALSO be the translation vector too.
    Vec2 calculatedMTV = minOverlap * axes[minIndex]; 


    // we have now found the MTV but there's a chance that the MTV will push INTO the shape.
    // to correct, we must check to see if the axis points in the same direction as the difference between other centroid and self centroid
    Vec2 towards = other.centroid() - centroid(); // this vector points from my center to the other's center
    // by property of dot product, if towards dot calculatedMTV is NOT <= 0: we have to flip it
    if (towards.dot(calculatedMTV) > 0) {
        calculatedMTV *= -1; // corrected
    }

    // only if the user wants an mtv, return it to thenm
    if (mtvOut != nullptr) {
        // update
        *mtvOut = calculatedMTV;
    }

    // return true because they ARE overlapping
    return true;
}

Vec2 Polygon::centroid() const {

    Vec2 c(0, 0);
    for (const Vec2& v : vertices) {
        c += v;
    }

    c /= vertices.size();

    return c;

}

void Polygon::moveBy(const Vec2 &dv) {

    for (Vec2& v : vertices) {
        v += dv;
    }

}

void Polygon::moveTo(const Vec2 &dest) {

    Vec2 difference = dest - centroid();

    for (Vec2& v : vertices) {
        v += difference;
    }

}

void Polygon::rotateBy(float da) {

    Vec2 centr = centroid();

    for (Vec2& v : vertices) {

        float x = v.x - centr.x;
        float y = v.y - centr.y;

        float xNew = x * std::cos(da) - y * std::sin(da);
        float yNew = x * std::sin(da) + y * std::cos(da);

        xNew += centr.x;
        yNew += centr.y;

        v = Vec2(xNew, yNew);

    }

    angle += da;
    angle = std::fmod(angle, 2 * PI);
    
}

void Polygon::rotateTo(float dest) {

    Vec2 centr = centroid();

    float da = dest - angle;

    for (Vec2& v : vertices) {

        float x = v.x - centr.x;
        float y = v.y - centr.y;

        float xNew = x * std::cos(da) - y * std::sin(da);
        float yNew = x * std::sin(da) + y * std::cos(da);

        xNew += centr.x;
        yNew += centr.y;

        v = Vec2(xNew, yNew);

    }

    angle += da;
    angle = std::fmod(angle, 2 * PI);
    
}
