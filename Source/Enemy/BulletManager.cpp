#include "BulletManager.h"

map<int, vector<Bullet*>> BulletManager::bulletList = map<int, vector<Bullet*>>();
map<int, vector<Bullet*>> BulletManager::unusedBulletList = map<int, vector<Bullet*>>();

void BulletManager::createBullet(int Type, Vector2 position, Vector2f direction, int speed) {
	Bullet* newBullet;
	if (unusedBulletList.contains(Type)) {
		if (unusedBulletList[Type].size() > 0) {
			newBullet = unusedBulletList[Type][0];
			unusedBulletList[Type].erase(unusedBulletList[Type].begin());
			//ReInit bullet
			newBullet->initialize(position, direction, speed);
		}
		else {
			newBullet = new Bullet();
			newBullet->initialize(position, direction, speed);
			//create new bullet
		}
	}
	else {
		bulletList[Type] = vector<Bullet*>();
		unusedBulletList[Type] = vector<Bullet*>();
		//create new bullet

		newBullet = new Bullet();
		newBullet->initialize(position, direction, speed);
	}
}

void BulletManager::disableBullet(int Type, int position) {
	if (bulletList.contains(Type)) {
		if (bulletList[Type].size() > position && bulletList[Type][position]->isActive) {
			bulletList[Type][position]->position.y = -999;
			bulletList[Type][position]->isActive = false;
			unusedBulletList[Type].push_back(bulletList[Type][position]);
		}
	}
}
void BulletManager::disableBullet(Bullet* target) {
	int Type = target->Type;
	if (bulletList.contains(Type)) {
		target->position.y = -999;
		target->isActive = false;
		unusedBulletList[Type].push_back(target);
	}
}