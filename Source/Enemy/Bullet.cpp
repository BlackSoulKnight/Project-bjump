#include "Bullet.h"
#include "../engine/World.h"

void Bullet::initialize(Vector2 position, Vector2f startDirection, int startSpeed) {
	isActive = true;
	this->position.x = position.x;
	this->position.y = position.y;
	direction = startDirection;
	speed = startSpeed;
}

void Bullet::update(int deltaT) {
	position.x += speed  * direction.x;
	position.y += speed  * direction.y;
}

Bullet::Bullet() :GameObject() {
	name = "Bullet";
	sprite.x = 3;
	sprite.y = 0;
	position.h = 16;
	position.w = 16;
	sprite.spiteSheetLocation = "../resources/Tilemap/monochrome_tilemap_transparent_packed.png";
	World::Instance->createGameObject(this);
};