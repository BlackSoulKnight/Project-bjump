#pragma once
#include "Utility.h"
#include "GameObject.h"

class Bullet:public GameObject
{
public:
	Vector2f direction;
	int speed;
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