#pragma once
#include <string>
#include <SDL.h>
#include <vector>
#include <map>
#include <iostream>
#include <sstream>
#include <fstream>
#include <iterator>
#include <sstream>
#define Math_PI           3.14159265358979323846
using namespace std;

const int SCALE = 5;
struct Vector3f {
	float x, y, z;
	Vector3f(float x, float y, float z) {
		this->x = x; this->y = y; this->z = z;
	}
};

struct Vector2f {
	float x, y;
	Vector2f(float x, float y) {
		this->x = x; this->y = y;
	}
	Vector2f() {
		x = 0;
		y = 0;
	}
	void normalize() {
		float length = sqrt(pow(this->x, 2) + pow(this->y, 2));
		if (length == 0) {
			length = 1;
		}
		this->x /= length;
		this->y /= length;
		
	}
	double len() {
		return sqrt(pow(this->x, 2) + pow(this->y, 2));
	}
	void operator+=(const Vector2f &r) {
		this->x += r.x;
		this->y += r.y;
		return;
	}
	const Vector2f& operator+(const Vector2f& r) {
		return Vector2f( this->x + r.x,this->y = r.y);
	}

	void rotateDegrees(double angle) {
		angle *= Math_PI / 180;
		double cs = cos(angle);
		double sn = sin(angle);
		double x = this->x * cs - this->y * sn;
		double y = this->x * sn + this->y * cs;
		this->x = x;
		this->y = y;
	}
	void rotate(double angle) {
		double cs = cos(angle);
		double sn = sin(angle);
		double x = this->x * cs - this->y * sn;
		double y = this->x * sn + this->y * cs;
		this->x = x;
		this->y = y;
	}

};

struct Vector2
{
	int x, y;

};


struct Sprite {
	string spiteSheetLocation;
	int x, y;
	int animationSize; // Minimum 0; Maximum X
	bool flipH, flipV;
};

double normalize(double min, double max, double val);