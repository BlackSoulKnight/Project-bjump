#include "Enemy.h"
#include "BulletManager.h"




Enemy::Enemy() :GameObject() {
	tag = "Enemy";

	// Test Bullet
	/*Bullet* s = new Bullet();
	B_Spiral* d = new B_Spiral();
	addBulletToList(s, { 1,1 }, 500, 600);
	addBulletToList(s, { -1,1 }, 500, 600);
	addBulletToList(s, { 1,-1 }, 500, 600);
	addBulletToList(d, { 0,1 }, 1500, 600);*/
}

void Enemy::update(int deltaT) {
	activeTime += deltaT;
	if (activeTime > begin) {
		for (int i = 0; i < bullets.size(); i++) {
			if (bulletDelay[i] + bulletLastShot[i] < activeTime) {
				bulletLastShot[i] = activeTime;
				BulletManager::createBullet(bullets[i], { position.x,position.y }, bulletDirection[i], bulletSpeed[i]);
			}
		}
	}
}

void Enemy::addBulletToList(Bullet* bullet, Vector2f _direction, int delay,int speed)
{
	bullets.push_back(bullet);
	bulletDirection.push_back(_direction);
	bulletDelay.push_back(delay);
	bulletSpeed.push_back(speed);
	bulletLastShot.push_back(0);
}

void Enemy::setBasicEnemy(Vector2f shotDirection, int speed) {
	addBulletToList(new Bullet(), shotDirection, 1000, speed);
	sprite.x = 3;
	sprite.y = 5;
}