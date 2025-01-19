#include "Enemy.h"
#include "BulletManager.h"




Enemy::Enemy() :GameObject() {
	tag = "Enemy";
}

void Enemy::update(int deltaT) {
	activeTime += (float)deltaT / 1000;
	if (activeTime > begin) {
		if (lastShot + delay < activeTime) {
			lastShot = activeTime;
			BulletManager::createBullet(0, { position.x,position.y }, { 1,1 }, 10);
		}
	}
}
