#include "Bullet.h"
#include "../engine/World.h"
#include "BulletManager.h"

void Bullet::initialize(Vector2 position, Vector2f startDirection, int startSpeed) {
	isActive = true;
	this->position.x = position.x;
	this->position.y = position.y;
	direction = startDirection;
	speed = startSpeed;
	activeTime = 0;
}

void Bullet::update(int deltaT) {
	hitCollision();
	moveBullet(deltaT);
	if (outOfScreen()) {
		BulletManager::disableBullet(this);
	}

}

Bullet::Bullet() :GameObject() {
	name = "Bullet";
	tag = "Bullet";
	sprite.x = 3;
	sprite.y = 0;
	position.h = 16;
	position.w = 16;
	sprite.spiteSheetLocation = "../resources/Tilemap/monochrome_tilemap_transparent_packed.png";
	//World::Instance->createGameObject(this);
};

bool Bullet::outOfScreen() {
	Camera camera = World::Instance->camera;
	if (camera.entityOutOfScreen(*this)) {
		return true;
	}
	return false;
}

Bullet* Bullet::createBullet() {
	return new Bullet();
}

bool Bullet::hitCollision()
{
	//if(World::Instance->collideWithPlayer(this))
		//BulletManager::disableBullet(this);


	//return false;


	vector<string> tags = {"Player"};
	vector<GameObject*> collisions = World::Instance->getCollisions(position,tags);
	if (collisions.size() > 0) {
		Player* player = (Player*)collisions[0];
		if (!player->gotHit) {
			player->gotHit = true;
			player->fallDirection = player->position.x + player->position.w / 2 -   position.x - position.w / 2;
			player->fallDirection /= abs(player->fallDirection);
		}
		BulletManager::disableBullet(this);
		return true;
		
	}
	return false;
}

void Bullet::moveBullet(int deltaT)
{
	position.x += (float)(speed * direction.x)/deltaT;
	position.y += (float)(speed * direction.y)/deltaT;
	activeTime += deltaT;
}
