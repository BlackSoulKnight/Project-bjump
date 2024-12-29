#pragma once
#include "Utility.h"


class GameObject
{
private:
	Vector3f position;
	Vector3f scale;
	Sprite sprite;
	
	
public:
	virtual void update(float deltaT);
	Vector3f getPosition() { return position; };
	Sprite getSprite() { return sprite; };
};

