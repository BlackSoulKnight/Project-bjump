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

struct Camera {
	int xPos, yPos;
	int xOffset, yOffset;

};