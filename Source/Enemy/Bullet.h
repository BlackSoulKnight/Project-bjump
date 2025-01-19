#pragma once
#include "Utility.h"
#include "GameObject.h"

class Bullet:public GameObject
{
public:
	Vector2f direction;
	int speed;
	Bullet();
	void initialize(Vector2 position,Vector2f startDirection,int startSpeed);
	void update(int deltaT);
	int Type;
	//Projectile trajactory inside ea bullet scrip invidual
	//Direction
	//Special movement function
	//Hit detection auf dem spieler realizieren
	//Sonstiges
};

