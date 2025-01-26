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
	vector<Bullet*> bullets;
	vector<Vector2f> bulletDirection;
	vector<int> bulletSpeed;
	vector<int> bulletDelay; // In milliseconds
	vector<int> bulletLastShot;
	vector<int> bulletBegin;
	int activeTime = 0;

	virtual void update(int deltaT);
	void addBulletToList(Bullet* bullet, Vector2f _direction, int delay, int speed, int begin = 0);


	Enemy();

	//Enemy Types
	void setBasicEnemy(Vector2f shotDirection,int speed);

};



//Lars Enemies

class L_E1 : public Enemy {
public:
	L_E1();
};
class L_E2 : public Enemy {
public:
	L_E2();

};
class L_E3 : public Enemy {
public:
	L_E3();

	void update(int deltaT);
};

class E5 : public Enemy {
public:
	E5();
};

class E7 : public Enemy {
public:
	E7();
};
class E9 : public Enemy {
public:
	E9();
};
