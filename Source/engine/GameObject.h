#pragma once
#include "Utility.h"
#include "InputManager.h"


class GameObject
{
private:
	
public:
	SDL_Rect position;
	Vector2f scale;
	Sprite sprite;
	string name = "";
	bool isActive;
	int layer;
	string tag;
	vector<GameObject*> collisions;
	GameObject();
	virtual void update(int deltaT);
	SDL_Rect getPosition() { return position; };
	Sprite getSprite() { return sprite; };
	vector<GameObject*> traceLine(Vector2 start, Vector2 end, bool includeSelf = false);
	vector<tuple<GameObject*, SDL_Rect>> getCollision();
};

class Player: public GameObject {
public:
	void update(int deltaT);
	void moveAndCollide(Vector2);
	Player();
	int speed = 1;
};