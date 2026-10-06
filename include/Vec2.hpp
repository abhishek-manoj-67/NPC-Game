#pragma once

#include <iostream>

class Vec2 {
public:
    // methods
    Vec2() : x(0), y(0) {};
    Vec2(float x_, float y_) : x(x_), y(y_) {};

    // unary
    float magnitude() const;
    float magnitudeSquared() const;
    float angle() const;

    Vec2 normalize() const;
    Vec2 perpendicular() const;

    Vec2& zero();

    Vec2 operator-() const;

    // binary
    float dot(const Vec2& other) const;
    float cross(const Vec2& other) const;

    Vec2& operator+=(const Vec2& other);
    Vec2& operator-=(const Vec2& other);
    Vec2& operator*=(const float scalar);
    Vec2& operator/=(const float scalar);

    Vec2 operator+(const Vec2& other) const;
    Vec2 operator-(const Vec2& other) const;
    Vec2 operator*(const float scalar) const;
    Vec2 operator/(const float scalar) const;

    // attrs
    float x, y;
};

inline Vec2 operator*(float scalar, const Vec2& vector) {

    return vector * scalar;

}

std::ostream& operator<<(std::ostream& os, const Vec2& vec);