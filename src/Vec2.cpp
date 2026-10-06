#include <iostream>
#include <cmath>
#include <cassert>
#include "Vec2.hpp"

float Vec2::magnitude() const {

    return sqrt(this->x * this->x + this->y * this->y);

}

float Vec2::magnitudeSquared() const {

    return this->x * this->x + this->y * this->y;

}

float Vec2::angle() const {

    return atan2(this->y, this->x);

}


Vec2 Vec2::normalize() const {
    
    float mag = this->magnitude();

    if (mag <= 1e-9) {
        return Vec2(0, 0);
    } else {
        return Vec2(this->x / mag, this->y / mag);
    }

}

Vec2 Vec2::perpendicular() const {

    return Vec2(-this->y, this->x);

}

Vec2& Vec2::zero() {

    this->x = 0;
    this->y = 0;

    return *this;

}

Vec2 Vec2::operator-() const {

    return Vec2(-this->x, -this->y);

}

float Vec2::dot(const Vec2& other) const {

    return (this->x * other.x + this->y * other.y);

}

float Vec2::cross(const Vec2& other) const {

    return (this->x * other.y - this->y * other.x);

}

Vec2& Vec2::operator+=(const Vec2& other) {

    this->x += other.x;
    this->y += other.y;

    return *this;

}

Vec2& Vec2::operator-=(const Vec2& other) {

    this->x -= other.x;
    this->y -= other.y;

    return *this;

}

Vec2& Vec2::operator*=(const float scalar) {

    this->x *= scalar;
    this->y *= scalar;

    return *this;

}

Vec2& Vec2::operator/=(const float scalar) {

    this->x /= scalar;
    this->y /= scalar;

    return *this;

}

Vec2 Vec2::operator+(const Vec2& other) const {

    return (Vec2(this->x + other.x, this->y + other.y));

}
Vec2 Vec2::operator-(const Vec2& other) const {

    return Vec2(this->x - other.x, this->y - other.y);

}

Vec2 Vec2::operator*(const float scalar) const {

    return Vec2(this->x * scalar, this->y * scalar);

}

Vec2 Vec2::operator/(const float scalar) const {

    assert(scalar != 0.0 && "Cannot divide by 0!");
    return Vec2(this->x / scalar, this->y / scalar);

}


std::ostream& operator<<(std::ostream& os, const Vec2& vec) {

    os << "(" << vec.x << ", " << vec.y << ")";
    return os;

}
