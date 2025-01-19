#pragma once
#include "GameObject.h"
#include "Utility.h"
#include "Bullet.h"


/*
Bullet Manager
-> creates or reuse bullets
-> destroy not used bullets
-> used by enemys
-> init bullets for reuse
-> Static class?
*/




class Enemy :public GameObject {
public:
	float begin = 1;
	float delay = 0.01;
	float lastShot = 0;
	float activeTime = 0;
	void update(int deltaT);
	Enemy();
	//bullet types (array)
	//bullet amounts (array)
	//bullet direction (array(vector2))
	//shoot delay
	//start delay

};
