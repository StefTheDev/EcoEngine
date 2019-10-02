#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <memory>
#include <typeindex>

#include <glew.h>
#include <glut.h>
#include <SOIL.h>
#include <time.h>
#include <ft2build.h>

#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>

#include FT_FREETYPE_H  


struct Vector2
{
	Vector2() : x(0.0f), y(0.0f) {}
	Vector2(float _x, float _y) : x(_x), y(_y) {	}

	Vector2& operator+=(const Vector2& v) { x += v.x; y += v.y; return *this; }
	Vector2& operator-=(const Vector2& v) { x -= v.x; y -= v.y; return *this; }
	Vector2& operator+=(const float& scalar) { x += scalar; y += scalar; return *this; }
	Vector2& operator-=(const float& scalar) { x -= scalar; y -= scalar; return *this; }

	friend Vector2 operator+(const Vector2& lhs, const Vector2& rhs) { return Vector2(lhs) += rhs; }
	friend Vector2 operator-(const Vector2& lhs, const Vector2& rhs) { return Vector2(lhs) -= rhs; }

	float x, y;
};