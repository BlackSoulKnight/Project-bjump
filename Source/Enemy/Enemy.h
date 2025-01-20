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
	vector<Bullet> bullets;
	vector<Vector2f> bulletDirection;
	vector<int> bulletSpeed;
	vector<int> bulletDelay; // In milliseconds
	vector<int> bulletLastShot;
	int activeTime = 0;

	void update(int deltaT);
	void addBulletToList(Bullet bullet,Vector2f _direction,int delay,int speed);

	Enemy();


};
