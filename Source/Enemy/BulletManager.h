#pragma once
#include "Utility.h"
#include "Bullet.h"

class BulletManager {
public:
	static map<int, vector<Bullet*>> bulletList;
	static map<int, vector<Bullet*>> unusedBulletList;

	static void createBullet(int Type, Vector2 position, Vector2f direction, int speed);
	static void disableBullet(int Type, int position);
	static void disableBullet(Bullet* target);


};