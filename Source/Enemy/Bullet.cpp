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
	if (false) {
		
		SDL_Rect collisionbox = targetHitbox;
		Camera camera = World::Instance->camera;
		SDL_Renderer* renderer = World::Instance->getRenderer();
		collisionbox.x +=  (camera.x + camera.xOffset);
		collisionbox.y +=(camera.y + camera.yOffset);
		SDL_SetRenderDrawColor(renderer, 0, 255, 0, 100);
		SDL_RenderFillRect(renderer, &collisionbox);
		SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
		SDL_RenderDrawRect(renderer, &collisionbox);
		SDL_RenderPresent(renderer);
	}

	vector<GameObject*> collisions = World::Instance->getCollisions(targetHitbox, tags);
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

void Bullet::moveBullet(float deltaT)
{
	position.x += (float)(speed * direction.x)*deltaT;
	position.y += (float)(speed * direction.y) *deltaT;
	activeTime += deltaT;
}

void B_Spiral::moveBullet(float deltaT) {
	//direction.x = 0.1+sin(activeTime);
	//direction.y = 1-direction.x;
	//direction.normalize(direction);
	direction.rotate(direction, 90*deltaT);
	activeTime += deltaT;
 	position.x += (float)(speed * direction.x) * deltaT;
	position.y += (float)(speed * direction.y)  * deltaT;
}

B_Spiral::B_Spiral() :Bullet() {
	Type = 1;
}