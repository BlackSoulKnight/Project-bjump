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
	moveBullet((float)deltaT/1000);
	if (outOfScreen()) {
		BulletManager::disableBullet(this);
	}

}

Bullet::Bullet() :GameObject() {
	name = "Bullet";
	tag = "Bullet";
	sprite.x = 1;
	sprite.y = 0;
	position.h = 32;
	position.w = 32;
	sprite.spiteSheetLocation = "../resources/Tilemap/monochrome_tilemap_transparent_packed.png";
	hitbox = { 5 * SCALE,5 * SCALE,6 * SCALE,6 * SCALE };
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
	SDL_Rect targetHitbox = hitbox;
	targetHitbox.x += position.x;
	targetHitbox.y += position.y;

	vector<GameObject*> collisions = World::Instance->getCollisions(targetHitbox, tags);
	if (collisions.size() > 0) {
		Player* player = (Player*)collisions[0];
		if (!player->gotHit) {
			player->fallDirection = player->position.x + player->position.w / 2 -   position.x - position.w / 2;
			if (player->fallDirection == 0) {
				player->fallDirection = -1;
			}
			player->fallDirection /= abs(player->fallDirection);
			player->gotHit = true;
		}
		BulletManager::disableBullet(this);
		return true;
		
	}
	return false;
}

void Bullet::moveBullet(float deltaT)
{
	Player* ply = World::Instance->player;
	Vector2f homing = direction + Vector2f(1, 1);
	position.x += (float)(speed * direction.x)*deltaT;
	position.y += (float)(speed * direction.y) *deltaT;
	activeTime += deltaT;
}

void B_Spiral::moveBullet(float deltaT) {
	//direction.x = 0.1+sin(activeTime);
	//direction.y = 1-direction.x;
	//direction.normalize(direction);
	direction.rotate(direction, 360*deltaT);
	speed += 100*deltaT;
	activeTime += deltaT;
 	position.x += (float)(speed * direction.x) * deltaT;
	position.y += (float)(speed * direction.y)  * deltaT;
}

B_Spiral::B_Spiral() :Bullet() {
	Type = 1;
}