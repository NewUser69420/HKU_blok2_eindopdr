#include <cmath>
#pragma once

class Vector2 {
public :
	float x = 0.f;
	float y = 0.f;

	Vector2() = default;

	Vector2(float a, float b) {
		x = a;
		y = b;
	}

	Vector2 operator*(const Vector2& b) const {
		return Vector2{ x * b.x, y * b.y };
	}
	Vector2 operator*(const float& b) const {
		return Vector2{ x * b, y * b };
	}

	Vector2 operator/(const Vector2& b) const {
		return Vector2{ x / b.x, y / b.y };
	}
	Vector2 operator/(const float& b) const {
		return Vector2{ x / b, y / b };
	}

	Vector2 operator+(const Vector2& b) const {
		return Vector2{ x + b.x, y + b.y };
	}

	Vector2 operator-(const Vector2& b) const {
		return Vector2{ x - b.x, y - b.y };
	}

	float mag() const {
		return sqrt(x * x + y * y);
	}

	float dist(const Vector2& b) const {
		return sqrt(abs(pow(b.x - x, 2.f)) + abs(pow(b.y - y, 2.f)));
	}
};