#pragma once

struct Vec2f;

struct Vec2 {
	int x, y;

	Vec2(int x, int y);
	Vec2(const Vec2& other);
	Vec2(const Vec2f& other);

	double size() const;

	Vec2 operator+(const Vec2& other) const;
	Vec2 operator-(const Vec2& other) const;
	Vec2 operator-() const;
	Vec2 operator*(double a) const;
	Vec2 operator/(double a) const;
	void operator+=(const Vec2& other);
	void operator-=(const Vec2& other);
};

struct Vec2f {
	Vec2f();
	Vec2f(float x, float y);
	Vec2f(const Vec2f& other);
	Vec2f(const Vec2& other);
	float x, y;

	Vec2f add(const Vec2f& other) const;
	Vec2f sub(const Vec2f& other) const;
	Vec2f mul(float a) const;
	Vec2f div(float a) const;
	float size() const;

	Vec2f operator+(const Vec2& other) const;
	Vec2f operator+(const Vec2f& other) const;
	Vec2f operator-(const Vec2& other) const;
	Vec2f operator-(const Vec2f& other) const;
	Vec2f operator-() const;
	Vec2f operator*(float a) const;
	Vec2f operator/(float a) const;
	void operator+=(const Vec2f& other);
	void operator-=(const Vec2f& other);
};

// Useful Maths
Vec2 AtoBProg(Vec2 a, Vec2 b, float prog);
