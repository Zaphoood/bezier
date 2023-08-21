#include "vector2.h"
#include <cmath>

// Vec2

Vec2::Vec2(int x_, int y_) : x(x_), y(y_) {}

Vec2::Vec2(const Vec2 &other) {
  x = other.x;
  y = other.y;
}

Vec2::Vec2(const Vec2f &other) {
  x = (int)other.x;
  y = (int)other.y;
}

Vec2 Vec2::operator+(const Vec2 &other) const {
  return Vec2(x + other.x, y + other.y);
}

Vec2 Vec2::operator-(const Vec2 &other) const {
  return Vec2(x - other.x, y - other.y);
}

Vec2 Vec2::operator-() const { return Vec2(-x, -y); }

Vec2 Vec2::operator*(double a) const {
  return Vec2((int)(x * a), (int)(y * a));
}

Vec2 Vec2::operator/(double a) const {
  return Vec2((int)(x / a), (int)(y / a));
}

void Vec2::operator+=(const Vec2 &other) {
  x += other.x;
  y += other.y;
}

void Vec2::operator-=(const Vec2 &other) {
  x -= other.x;
  y -= other.y;
}

double Vec2::size() const { return sqrt(x * x + y * y); }

Vec2f::Vec2f() : x(0.0f), y(0.0f) {}
Vec2f::Vec2f(float x_, float y_) : x(x_), y(y_) {}
Vec2f::Vec2f(const Vec2f &other) {
  x = other.x;
  y = other.y;
}
Vec2f::Vec2f(const Vec2 &other) {
  x = (float)other.x;
  y = (float)other.y;
}

Vec2f Vec2f::add(const Vec2f &other) const {
  return Vec2f(x + other.x, y + other.y);
}
Vec2f Vec2f::sub(const Vec2f &other) const {
  return Vec2f(x - other.x, y - other.y);
}
Vec2f Vec2f::mul(float a) const { return Vec2f(x * a, y * a); }
Vec2f Vec2f::div(float a) const { return Vec2f(x / a, y / a); }
float Vec2f::size() const { return sqrt(x * x + y * y); }

Vec2f Vec2f::operator+(const Vec2 &other) const { return add(other); }
Vec2f Vec2f::operator+(const Vec2f &other) const { return add(other); }
Vec2f Vec2f::operator-(const Vec2 &other) const { return sub(other); }
Vec2f Vec2f::operator-(const Vec2f &other) const { return sub(other); }
Vec2f Vec2f::operator-() const { return Vec2f(-x, -y); }
Vec2f Vec2f::operator*(float a) const { return mul(a); }
Vec2f Vec2f::operator/(float a) const { return div(a); }
void Vec2f::operator+=(const Vec2f &other) {
  x += other.x;
  y += other.y;
}
void Vec2f::operator-=(const Vec2f &other) {
  x -= other.x;
  y -= other.y;
}

// Useful Maths
Vec2 AtoBProg(Vec2 a, Vec2 b, float prog) { return a + (b - a) * prog; }
