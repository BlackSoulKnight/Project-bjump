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
		for (int i = 0; i < bulletLastShot.size(); i++) {
			bulletLastShot[i] = 0;
		}
		return;
	}
	activeTime += deltaT;
	if (activeTime > begin) {
		for (int i = 0; i < bullets.size(); i++) {
			if (bulletDelay[i] * (activeTime / begin -1 > 1 || bulletLastShot[i] >0) + bulletLastShot[i] + bulletBegin[i] < activeTime) {
				bulletLastShot[i] = activeTime;
				BulletManager::createBullet(bullets[i], { position.x,position.y }, bulletDirection[i], bulletSpeed[i]);
			}
		}
	}
}

void Enemy::addBulletToList(Bullet* bullet, Vector2f _direction, int delay, int speed,int begin)
{
	bullets.push_back(bullet);
	bulletDirection.push_back(_direction);
	bulletDelay.push_back(delay);
	bulletSpeed.push_back(speed);
	bulletBegin.push_back(begin);
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
E5::E5() :Enemy() {
	begin = 1000;
	addBulletToList(new B_Spiral(), { -1,0 }, 4500, 500);
	addBulletToList(new B_Spiral(), { 1,0 }, 4500, 500);
	addBulletToList(new B_Spiral(), { 0,1 }, 4500, 500);
	addBulletToList(new B_Spiral(), { 0,-1 }, 4500, 500);
}
E6::E6() :Enemy() {
	begin = 1000;
	addBulletToList(new B_Spiral(), { -1,0 }, 4500, -2000);
	addBulletToList(new B_Spiral(), { 1,0 }, 4500, -2000);
	addBulletToList(new B_Spiral(), { 0,1 }, 4500, -2000);
	addBulletToList(new B_Spiral(), { 0,-1 }, 4500, -2000);
}

E7::E7() :Enemy() {
	begin = 1000;
	addBulletToList(new B_HSinus(), { 1,0 }, 500, 500);

}
E8::E8() :Enemy() {
	begin = 1000;
	addBulletToList(new B_HSinus(), { -1,0 }, 500, 500);

}
E9::E9() :Enemy() {
	begin = 200;
	addBulletToList(new B_Homing(), { 0,-1 }, 750, 600);

}
E10::E10() :Enemy() {
	begin = 1000;
	addBulletToList(new B_Firework(), { 0,-1}, 3000, 600);

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
