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
		//BulletManager::disableBullet(this);
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
	if (activeTime > 20 * 1000)
		return true;
	else 
		return false;
	int w, h; // this needs to be reworked
	SDL_RenderGetLogicalSize(World::Instance->getRenderer(), &w, &h);
	if (!(camera.x + camera.w + w/2 < position.x && position.x < camera.x - position.w + w/2) ||
		(!(-camera.y + camera.h + h/2  < position.y && position.y < -camera.y - position.h + h))) {
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


	vector<string> tags = { "Player"};
	vector<tuple<GameObject*, SDL_Rect>> collisions = World::Instance->getCollisions(this,tags);
	if (World::Instance->getCollisions(this).size() > 0) {
		for (int i = 0; i < collisions.size(); i++) {
			if (get<0>(collisions[i])->tag != "Enemy") {
				BulletManager::disableBullet(this);
				return true;
			}
		}
	}
	return false;
}

void Bullet::moveBullet(int deltaT)
{
	position.x += (float)(speed * direction.x)/deltaT;
	position.y += (float)(speed * direction.y)/deltaT;
	activeTime += deltaT;
}
