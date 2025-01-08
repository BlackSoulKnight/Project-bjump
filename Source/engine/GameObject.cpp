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

vector<tuple<GameObject*, SDL_Rect>> GameObject::getCollision() {
	return World::Instance->getCollisions(this);
}


void Player::update(int deltaT) {
	map<string, bool> inputs = InputManager::getInput();
	if (collisions.size() > 0) {
		cout << collisions[0]->name;
	}
	vector<tuple<GameObject*, SDL_Rect>> test = getCollision();
	for (int i = 0; i < test.size(); i++) {
		cout << get<0>(test[i])->name;
	}
	Vector2 movement = Vector2(0,0);
	if(inputs.contains("Right") && inputs["Right"])
		movement.x += speed*deltaT;
	if(inputs.contains("Left") && inputs["Left"])
		movement.x -= speed*deltaT;
	if (inputs.contains("Jump") && inputs["Jump"])
		movement.y += speed*deltaT;
	else if (position.y > 0)
		movement.y -= 1;
	moveAndCollide(movement);
}

Player::Player()
{
	position.x = 50;
	position.w = 100;
	position.h = 100;
	name = "player";
}

void Player::moveAndCollide(Vector2 direction) {

	vector<tuple<GameObject*, SDL_Rect>> collisions = getCollision();
	//x
	position.x += direction.x;
	while ((collisions = getCollision()).size() > 0) {

		if (position.x + position.w / 2 > get<1>(collisions[0]).x)
		{
			position.x += get<1>(collisions[0]).w;
		}
		else {
			position.x -= get<1>(collisions[0]).w;
		}
	}
	
	//y
	position.y += direction.y;
	while ((collisions = getCollision()).size() > 0) {

		if (position.y + position.h / 2 > get<1>(collisions[0]).y)
		{
			position.y += get<1>(collisions[0]).h;
		}
		else {
			position.y -= get<1>(collisions[0]).h;
		}
	}
}