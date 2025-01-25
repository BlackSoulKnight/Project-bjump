#include "Enemy.h"
#include "BulletManager.h"
#include "../engine/World.h"



Enemy::Enemy() :GameObject() {
	tag = "Enemy";
	sprite.x = 3;
	sprite.y = 5;
	sprite.spiteSheetLocation = "../resources/Tilemap/monochrome_tilemap_transparent_packed.png";

	// Test Bullet
	/*Bullet* s = new Bullet();
	B_Spiral* d = new B_Spiral();
	addBulletToList(s, { 1,1 }, 500, 600);
	addBulletToList(s, { -1,1 }, 500, 600);
	addBulletToList(s, { 1,-1 }, 500, 600);
	addBulletToList(d, { 0,1 }, 1500, 600);*/
}

void Enemy::update(int deltaT) {
	if (World::Instance->camera.entityOutOfScreen(*this)) {
		activeTime = 0;
		return;
	}
	activeTime += deltaT;
	if (activeTime > begin) {
		for (int i = 0; i < bullets.size(); i++) {
			if (bulletDelay[i] + bulletLastShot[i] + begin < activeTime) {
				bulletLastShot[i] = activeTime;
				BulletManager::createBullet(bullets[i], { position.x,position.y }, bulletDirection[i], bulletSpeed[i]);
			}
		}
	}
}

void Enemy::addBulletToList(Bullet* bullet, Vector2f _direction, int delay, int speed)
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

L_E1::L_E1():Enemy() {
	begin = 1000;
	addBulletToList(new Bullet(), { 0,1 }, 2000, 600);
}
L_E2::L_E2() :Enemy() {
	begin = 2000;
	addBulletToList(new Bullet(), { 0,1 }, 2000, 600);
}
L_E3::L_E3() :Enemy() {
	begin = 1000;
	addBulletToList(new Bullet(), { -1,0 }, 300, 600);
}

void L_E3::update(int deltaT)
{
	activeTime += deltaT;
	if (activeTime > begin) {
		for (int i = 0; i < bullets.size(); i++) {
			if (bulletDelay[i] + bulletLastShot[i] + begin < activeTime) {
				bulletLastShot[i] = activeTime;
				BulletManager::createBullet(bullets[i], { position.x,position.y }, bulletDirection[i], bulletSpeed[i]);
				bulletDelay[i] = 100 + rand() % 300;
			}
		}
	}
}
