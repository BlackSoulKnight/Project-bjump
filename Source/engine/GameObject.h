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
	bool flipH, flipV;
	vector<GameObject*> collisions;
	GameObject();
	virtual void update(int deltaT);
	SDL_Rect getPosition() { return position; };
	Sprite getSprite() { return sprite; };
};

class Player: public GameObject {
public:
	void update(int deltaT);
	Player();
	int speed = 3;
};