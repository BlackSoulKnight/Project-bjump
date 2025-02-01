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
	SDL_Rect hitbox = { 0,0,16 * SCALE,16 * SCALE };
	bool isActive;
	int layer;
	string tag;
	vector<GameObject*> collisions;
	long int frameCount;
	GameObject();
	virtual void update(int deltaT);
	SDL_Rect getPosition() { return position; };
	Sprite getSprite() { return sprite; };
	Sprite* getSpriteP() { return &sprite; };
	vector<GameObject*> traceLine(Vector2 start, Vector2 end, bool includeSelf = false);
	vector<tuple<GameObject*, SDL_Rect>> getCollision();
	bool getInputState(string Action);
};
