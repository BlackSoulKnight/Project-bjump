#include "GameObject.h"

void GameObject::update(int deltaT) {
	
}

GameObject::GameObject() {
	position.x = 0;
	position.y = 0;
	position.w = 0;
	position.h = 0;
	isActive = true;
}
void Player::update(int deltaT) {
	map<string, bool> inputs = InputManager::getInput();
	if (collisions.size() > 0) {
		cout << collisions[0]->name;
	}
	if(inputs.contains("Right") && inputs["Right"])
		position.x += speed*deltaT;
	if(inputs.contains("Left") && inputs["Left"])
		position.x -= speed*deltaT;
	
}

Player::Player() {
	position.w = 100;
	position.h = 100;
}