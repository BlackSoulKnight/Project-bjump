#include "GameObject.h"
#include "World.h"
void GameObject::update(int deltaT) {
	
}

GameObject::GameObject() {
	position.x = 0;
	position.y = 0;
	position.w = 0;
	position.h = 0;
	isActive = true;
	layer = 0;
	tag = "";
}


vector<GameObject*> GameObject::traceLine(Vector2 start, Vector2 end, bool includeSelf) {
	vector<GameObject*> result = World::Instance->getTraceCollisions(start, end);
	if (!includeSelf) {
		for (int i = 0; i < result.size(); i++) {
			if (result[i] == this) {
					result.erase(result.begin() + i);
			}
		}
	}
	return result;
}

void Player::update(int deltaT) {
	map<string, bool> inputs = InputManager::getInput();
	if (collisions.size() > 0) {
		cout << collisions[0]->name;
	}
	vector<GameObject*> t = traceLine(Vector2(this->position.x, this->position.y), Vector2(this->position.x+30, this->position.y));
	for (int i = 0; i < t.size(); i++) {
		cout << t[i]->name;
	}
	if(inputs.contains("Right") && inputs["Right"])
		position.x += speed*deltaT;
	if(inputs.contains("Left") && inputs["Left"])
		position.x -= speed*deltaT;
}

Player::Player() {
	position.w = 100;
	position.h = 100;
	name = "player";
}