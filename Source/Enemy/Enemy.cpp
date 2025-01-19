#include "Enemy.h"
#include "BulletManager.h"




Enemy::Enemy() :GameObject() {
	name = "Enemy";
}

void Enemy::update(int deltaT) {
	activeTime += (float)deltaT / 1000;
	if (activeTime > begin) {
		if (lastShot < activeTime / delay) {
			lastShot += delay;
			BulletManager::createBullet(0, { position.x,position.y }, { 0,1 }, 10);
		}
	}
}
