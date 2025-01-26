#pragma once
#include "Utility.h"
#include "GameObject.h"

class Bullet:public GameObject
{
public:
	Vector2f direction;
	double speed;
	float activeTime = 0;
	Bullet();
	virtual Bullet* createBullet();
	virtual bool hitCollision();
	virtual void moveBullet(float deltaT);
	void initialize(Vector2 position,Vector2f startDirection,int startSpeed);
	void update(int deltaT);
	bool outOfScreen();
	int Type = 0;
	//Projectile trajactory inside ea bullet scrip invidual
	//Direction
	//Special movement function
	//Hit detection auf dem spieler realizieren
	//Sonstiges
};

class B_Spiral : public Bullet
{
public:
	B_Spiral();
	void moveBullet(float deltaT);
	Bullet* createBullet() { return new B_Spiral; };
};
class B_HSinus : public Bullet
{
public:
	B_HSinus();
	void moveBullet(float deltaT);
	Bullet* createBullet() { return new B_HSinus; };
};
class B_Homing : public Bullet
{
public:
	float homingFactor = 1;
	B_Homing();
	void moveBullet(float deltaT);
	Bullet* createBullet() { return new B_Homing; };
};

class B_Random_Normal : public Bullet {
	B_Random_Normal();
	Bullet* createBullet();
};