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

bool GameObject::getInputState(string Action) {
	map<string, bool> inputs = InputManager::getInput();
	if (!inputs.contains(Action)) {
		cout << "Action ist nicht definiert";
	}
	return inputs[Action];
}

