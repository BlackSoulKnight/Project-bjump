#pragma once
#include <string>
using namespace std;

struct Vector3f {
	float x, y, z;
	Vector3f(float x, float y, float z) {
		this->x = x; this->y = y; this->z = z;
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
	bool flipped;
};