#include "BulletManager.h"
#include "../engine/World.h"

map<int, vector<Bullet*>> BulletManager::bulletList = map<int, vector<Bullet*>>();
map<int, vector<Bullet*>> BulletManager::unusedBulletList = map<int, vector<Bullet*>>();

void BulletManager::createBullet(Bullet* bullet, Vector2 position, Vector2f direction, int speed) {
	Bullet* newBullet;
	if (unusedBulletList.contains(bullet->Type)) {
		if (unusedBulletList[bullet->Type].size() > 0) {
			newBullet = unusedBulletList[bullet->Type][unusedBulletList[bullet->Type].size() - 1];
			unusedBulletList[bullet->Type].erase(unusedBulletList[bullet->Type].end()-1);
			//ReInit bullet
			newBullet->initialize(position, direction, speed);
		}
		else {
			newBullet = bullet->createBullet();
			World::Instance->createGameObject(newBullet);
			newBullet->initialize(position, direction, speed);
			//create new bullet
		}
	}
	else {
		bulletList[bullet->Type] = vector<Bullet*>();
		unusedBulletList[bullet->Type] = vector<Bullet*>();
		//create new bullet

		newBullet = bullet->createBullet();
		World::Instance->createGameObject(newBullet);
		newBullet->initialize(position, direction, speed);
	}
}

void BulletManager::disableBullet(int Type, int position) {
	if (!unusedBulletList.contains(Type)) {
		unusedBulletList[Type] = vector<Bullet*>();
	}
	if (bulletList[Type].size() > position && bulletList[Type][position]->isActive) {
		bulletList[Type][position]->position.y = -999;
		bulletList[Type][position]->isActive = false;
		unusedBulletList[Type].push_back(bulletList[Type][position]);
	}

}
void BulletManager::disableBullet(Bullet* target) {
	int Type = target->Type;
	if (!unusedBulletList.contains(Type)) {
		unusedBulletList[Type] = vector<Bullet*>();
	}
	target->position.y = -999;
	target->isActive = false;
	unusedBulletList[Type].push_back(target);

}