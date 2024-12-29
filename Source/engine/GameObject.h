#pragma once
#include "Utility.h"

class GameObject
{
private:
	Vector3f position;
	Vector3f scale;

	
public:
	virtual void update(float deltaT);
};

